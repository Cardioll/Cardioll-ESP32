#include <Cardioll.h>


void Cardioll::update(float sample) 
{
    buffer_.push(sample);
}

void Cardioll::processEcg() {
    unsigned long current_time = millis();
        
    if (current_time - last_r_peak_time_ >= QRS_DETECTION_WINDOW) {
        r_peak_detected_ = false;
    }
    if (current_time - last_rr_calculation_time_ >= RR_LOW_LIMIT) {
        rr_peak_detected_ = false;
    }
        
    if (buffer_.isFull() || last_r_peak_detected_ != 0) {
        if (peak_i == 0)
            preprocess();
        detectRPeaks();
        if (r_peak_detected_) {
            last_r_peak_time_ = current_time;
            detectQSWaves();
            detectPTWaves();
            calculateFeatures();
            if (rr_peak_detected_) {
                last_rr_calculation_time_ = current_time;
            }
        }
    }
}