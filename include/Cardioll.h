#ifndef __CARDIOLL_H_
#define __CARDIOLL_H_ 

#include <Arduino.h>
#include <CircularBuffer.hpp>
#include <FIR.h>

#define SAMPLING_RATE 360
#define BUFFER_LENGTH (3 * SAMPLING_RATE)
#define LO_POS 13
#define LO_NEG 14
#define LED_OUT 18
#define INPUT_PIN 34
#define LOCKOUT_TIME 200
#define WINDOW_SIZE 30
#define QRS_DETECTION_WINDOW 72
#define LEARNING_RATE 0.125
#define RR_LOW_LIMIT 200
#define RR_HIGH_LIMIT 1200
#define RR_MISSED_LIMIT 1600
#define LOW_PASS_FILTER_ORDER 30
#define HIGH_PASS_FILTER_ORDER 6
#define MOVINGAVERAGE_DELAY 5
#define P_WAVE_WINDOW 70 
#define T_WAVE_WINDOW 130  

// Debug control
//#define DEBUG_TELEPLOT  // Comment out to disable Teleplot output
//#define DEBUG_SERIAL    // Comment out to disable Serial debug output

class Cardioll {
    public:
        Cardioll();
        struct ECGFeatures {
            float pr_interval; 
            float qrs_duration; 
            float qt_interval; 
            float qtc_interval; 
            float qs_interval; 
            float rr_interval;
            float average_rr_interval; // Added for RR averaging
            float heart_rate;
        };
        void setup();
        void update(float sample);
        bool IsRpeakDetected() const;
        void processEcg();
        ECGFeatures GetFeatures() const;

    private:
        unsigned long last_r_peak_time_ = 0;
        unsigned long last_rr_calculation_time_ = 0;

        CircularBuffer<float, BUFFER_LENGTH> buffer_;
        CircularBuffer<float, BUFFER_LENGTH> temp_buffer_;
        volatile bool buffer_ready_ = false; 
        volatile bool processing_ = false;

        // Filters 
        FIR<float, 13> fir_lp; 
        FIR<float, 13> fir_hp; 
        FIR<float, 10> fir_avg;

        // Pan-Tompkins algorithm steps
        float spki_; 
        float npki_; 
        float threshold1_; 
        float threshold2_; 

        // Wave-specific 
        bool r_peak_detected_; 
        bool rr_peak_detected_;
        int last_r_peak_detected_;
        int prev_r_peak_index;
        int peak_i;
        ECGFeatures features_; 

        // RR Interval averaging
        static const int RR_HISTORY_SIZE = 8;
        float rr_history_[RR_HISTORY_SIZE] = {0};
        int rr_history_index_ = 0;
        bool rr_history_filled_ = false;

        // Preprocessing methods
        void preprocess(); 
        void detrend(); 
        void applyDigitalFilters();
        void applyMovingAverageFilter();

        // Detection methods
        void detectRPeaks(); 
        void detectQSWaves(); 
        void detectPTWaves(); 
        void calculateFeatures();

        // Helper methods
        float findExtremum(int start_index, int end_index, bool find_maximum);
        float calculateAverageRR();
        void updateRRHistory(float new_rr);

        // Debug methods
        #if defined(DEBUG_TELEPLOT) || defined(DEBUG_SERIAL)
        void debugPrint(const char* label, float value);
        #endif

        #ifdef DEBUG_TELEPLOT
        void telePlotSignal(const char* name, float value, int index);
        void telePlotMarker(const char* name, int index);
        #endif
};

#endif