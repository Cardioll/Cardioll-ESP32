#include <Cardioll.h>

void Cardioll::preprocess() {
    for (int i = 0; i < BUFFER_LENGTH; i++) {
        // 1. Get moving-average filtered value
        float sample = buffer_.shift();
//       float value = fir_avg.processReading(sample);
        #ifdef DEBUG_TELEPLOT
        telePlotSignal("raw_signal", sample, i);
        #endif
        // 3. Apply bandpass filtering (low-pass then high-pass)
        float value = fir_lp.processReading(sample);

        #ifdef DEBUG_TELEPLOT
        telePlotSignal("lowpass_signal", value, i);
        #endif

        value = fir_hp.processReading(value);

        #ifdef DEBUG_TELEPLOT
        telePlotSignal("bandpass_signal", value, i);
        #endif

        temp_buffer_.push(value);

        yield();
    }
}