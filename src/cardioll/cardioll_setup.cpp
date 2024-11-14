# include <Cardioll.h>

float kLowPassCoeffs[LOW_PASS_FILTER_ORDER + 1] = {
    1.69224642e-03, 1.76750779e-03, 1.46162606e-03, -1.08705488e-18,
    -3.34891607e-03, -8.51838378e-03, -1.40263319e-02, -1.68965202e-02,
    -1.33283238e-02,  4.41776868e-18,  2.44318077e-02,  5.82410197e-02,
    9.64736801e-02, 1.31929281e-01, 1.57053355e-01, 1.66135904e-01,
    1.57053355e-01, 1.31929281e-01, 9.64736801e-02, 5.82410197e-02,
    2.44318077e-02, 4.41776868e-18, -1.33283238e-02, -1.68965202e-02,
    -1.40263319e-02, -8.51838378e-03, -3.34891607e-03, -1.08705488e-18,
    1.46162606e-03, 1.76750779e-03, 1.69224642e-03
};

float kHighPassCoeffs[HIGH_PASS_FILTER_ORDER + 1] = {
    -1.11095594e-04, -4.30502258e-04, -1.06932224e-03, 9.98500169e-01,
    -1.06932224e-03, -4.30502258e-04, -1.11095594e-04
};

float kAvegrageCoeffs[MOVINGAVERAGE_DELAY] = {1., 1., 1., 1., 1.};

Cardioll::Cardioll() : 
    spki_(0), npki_(0), threshold1_(0), threshold2_(0),
    r_peak_detected_(false), rr_peak_detected_(false), last_r_peak_detected_(-1), 
    prev_r_peak_index(-1), peak_i(0) {
    // Initialize features
    features_ = {0};
}

void Cardioll::setup() {
    buffer_.clear();

    pinMode(INPUT_PIN, INPUT);
    pinMode(LO_POS, INPUT);
    pinMode(LO_NEG, INPUT);
    pinMode(LED_OUT, OUTPUT);
    fir_lp.setFilterCoeffs(kLowPassCoeffs); 
    fir_hp.setFilterCoeffs(kHighPassCoeffs); 
    fir_avg.setFilterCoeffs(kAvegrageCoeffs);

    // Set the gain
    Serial.print("Low Pass Filter Gain: ");
    Serial.println(fir_lp.getGain());
    Serial.print("High Pass Filter Gain: "); 
    Serial.println(fir_hp.getGain());
    Serial.print("Moving Average Filter Gain: ");
    Serial.println(fir_avg.getGain());
}