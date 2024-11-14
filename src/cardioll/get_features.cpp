# include <Cardioll.h>

bool Cardioll::IsRpeakDetected() const {
    return rr_peak_detected_; 
}

Cardioll::ECGFeatures Cardioll::GetFeatures() const {
    return features_;
}