# include <Cardioll.h>

void Cardioll::detectRPeaks() {
    const int DERIVATIVE_DELAY = 4;
    const int INTEGRATION_WINDOW = 54; // 150ms at 360Hz
    const float MIN_PEAK_HEIGHT = 0.5; // Adjust based on your signal amplitude
    const int MIN_PEAK_DISTANCE = 200; // Minimum 300ms between peaks (72 samples at 360Hz)
    
    static float moving_window_sum = 0;
    static float mwi_buffer[INTEGRATION_WINDOW] = {0};
    static int mwi_index = 0;   
    
    // Variables for peak detection
    float max_integrated = 0;
    int potential_peak_index = -1;
    int samples_since_last_peak = peak_i - last_r_peak_detected_;
    
    for (; peak_i < BUFFER_LENGTH - DERIVATIVE_DELAY; peak_i++) {
        float derivative = 2 * temp_buffer_[peak_i] + temp_buffer_[peak_i - 1] 
            - temp_buffer_[peak_i - 3] - 2 * temp_buffer_[peak_i - 4];
        
        float squared = derivative * derivative;
        
        // Moving Window Integration
        moving_window_sum -= mwi_buffer[mwi_index];
        moving_window_sum += squared;
        mwi_buffer[mwi_index] = squared;
        mwi_index = (mwi_index + 1) % INTEGRATION_WINDOW;
        
        float integrated = moving_window_sum / INTEGRATION_WINDOW;
        
        #ifdef DEBUG_TELEPLOT
        telePlotSignal("derivative_signal", derivative, peak_i);
        telePlotSignal("squared_signal", squared, peak_i);
        telePlotSignal("integrated_signal", integrated, peak_i);
        telePlotSignal("threshold", threshold2_, peak_i);
        #endif
        
        samples_since_last_peak++;
        
        // Peak detection logic
        if (integrated > threshold2_ && integrated > max_integrated && 
            samples_since_last_peak >= MIN_PEAK_DISTANCE) {
            max_integrated = integrated;
            potential_peak_index = peak_i;
        } else if (potential_peak_index >= 0 && 
                   (integrated < threshold2_ || peak_i - potential_peak_index > INTEGRATION_WINDOW)) {
            // Confirm peak
            if (max_integrated > MIN_PEAK_HEIGHT) {
                spki_ = LEARNING_RATE * max_integrated + (1 - LEARNING_RATE) * spki_;
                
                #ifdef DEBUG_TELEPLOT
                telePlotMarker("r_peak", potential_peak_index);
                #endif
                
                #ifdef DEBUG_SERIAL
                Serial.println("QRS DETECTED");
                #endif
                
                r_peak_detected_ = true;
                last_r_peak_detected_ = potential_peak_index;
                break;
            }
            
            // Reset peak detection
            max_integrated = 0;
            potential_peak_index = -1;
        }
        
        if (integrated <= threshold2_) {
            npki_ = LEARNING_RATE * integrated + (1 - LEARNING_RATE) * npki_;
        }
        
        threshold1_ = npki_ + 0.25 * (spki_ - npki_);
        threshold2_ = 0.5 * threshold1_;
        
        yield();
    }
    
    if (peak_i == (BUFFER_LENGTH - DERIVATIVE_DELAY)) 
    {
        last_r_peak_detected_ = 0;
        prev_r_peak_index = -1;
        peak_i = 0;
    }
}

void Cardioll::detectQSWaves() {
    if (!r_peak_detected_ || last_r_peak_detected_ < 20 
        || last_r_peak_detected_ >= BUFFER_LENGTH - 25) {
        return;
    }

    int q_start = max(0, last_r_peak_detected_ - 20);
    int q_end = last_r_peak_detected_ - 5;
    int s_start = last_r_peak_detected_ + 1;
    int s_end = min(BUFFER_LENGTH - 1, last_r_peak_detected_ + 25);
    
    float q_wave = findExtremum(q_start, q_end, false);
    float s_wave = findExtremum(s_start, s_end, false);
    
    if (q_wave >= 0 && s_wave >= 0) {
        features_.qs_interval = (s_wave - q_wave) * 1000.0 / SAMPLING_RATE;
        
        #ifdef DEBUG_TELEPLOT
        telePlotMarker("q_wave", q_wave);
        telePlotMarker("s_wave", s_wave);
        #endif
    }
}

void Cardioll::detectPTWaves() {
    if (!r_peak_detected_ || last_r_peak_detected_ < P_WAVE_WINDOW 
        || last_r_peak_detected_ >= BUFFER_LENGTH - T_WAVE_WINDOW) {
        return;
    }

    float current_rr = features_.average_rr_interval > 0 ? 
        features_.average_rr_interval : (features_.rr_interval > 0 ? features_.rr_interval : 1000);
    
    int p_start = last_r_peak_detected_ - min(P_WAVE_WINDOW, int(current_rr * 0.4));
    int p_end = last_r_peak_detected_ - 15;
    int t_start = last_r_peak_detected_ + 25;
    int t_end = last_r_peak_detected_ + min(T_WAVE_WINDOW, int(current_rr * 0.6));
    
    float p_wave = findExtremum(p_start, p_end, true);
    float t_wave = findExtremum(t_start, t_end, true);
    
    if (p_wave >= 0) {
        features_.pr_interval = (last_r_peak_detected_ - p_wave) * 1000.0 / SAMPLING_RATE;
        #ifdef DEBUG_TELEPLOT
        telePlotMarker("p_wave", p_wave);
        #endif
    }
    
    if (t_wave >= 0) {
        features_.qt_interval = (t_wave - last_r_peak_detected_) * 1000.0 / SAMPLING_RATE;
        #ifdef DEBUG_TELEPLOT
        telePlotMarker("t_wave", t_wave);
        #endif
    }
}

float Cardioll::calculateAverageRR() {
    if (!rr_history_filled_) {
        int valid_samples = 0;
        float sum = 0;
        for (int i = 0; i < RR_HISTORY_SIZE; i++) {
            if (rr_history_[i] > 0) {
                sum += rr_history_[i];
                valid_samples++;
            }
            yield();
        }
        return valid_samples > 0 ? sum / valid_samples : 0;
    }
    
    float sum = 0;
    for (int i = 0; i < RR_HISTORY_SIZE; i++) {
        sum += rr_history_[i];
        yield();
    }
    return sum / RR_HISTORY_SIZE;
}

void Cardioll::updateRRHistory(float new_rr) {
    if (new_rr < RR_LOW_LIMIT || new_rr > RR_HIGH_LIMIT) {
        return;
    }
    
    float current_avg = calculateAverageRR();
    if (current_avg > 0) {
        if (new_rr < current_avg * 0.5 || new_rr > current_avg * 1.5) {
            return;
        }
    }
    
    rr_history_[rr_history_index_] = new_rr;
    rr_history_index_ = (rr_history_index_ + 1) % RR_HISTORY_SIZE;
    
    if (!rr_history_filled_ && rr_history_index_ == 0) {
        rr_history_filled_ = true;
    }
}

void Cardioll::calculateFeatures() {
    if (!r_peak_detected_ || last_r_peak_detected_ < 1) {
        return;
    }


    if (prev_r_peak_index != -1) {
        float current_rr = (last_r_peak_detected_ - prev_r_peak_index) * 1000.0 / SAMPLING_RATE;
        
        updateRRHistory(current_rr);
        features_.average_rr_interval = calculateAverageRR();
        features_.rr_interval = current_rr;
        
        if (features_.average_rr_interval > 0) {
            features_.heart_rate = 60000.0 / features_.average_rr_interval;
        }
        
        rr_peak_detected_ = true;
    }
    
    prev_r_peak_index = last_r_peak_detected_;
    
    features_.qrs_duration = features_.qs_interval;
    
    float rr_for_qtc = features_.average_rr_interval > 0 ? 
        features_.average_rr_interval : features_.rr_interval;

    if (rr_for_qtc > 0) {
        features_.qtc_interval = features_.qt_interval / sqrt(rr_for_qtc / 1000.0);
    }
    
    #ifdef DEBUG_SERIAL
    debugPrint("Heart Rate: ", features_.heart_rate);
    debugPrint("RR Interval: ", features_.rr_interval);
    debugPrint("Avg RR Interval: ", features_.average_rr_interval);
    debugPrint("QRS Duration: ", features_.qrs_duration);
    #endif
}