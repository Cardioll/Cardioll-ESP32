#include "cardiollTClient.h"
#include "Cardioll.h"

#define _TIMERINTERRUPT_LOGLEVEL_ 0

#define SAMPLE_INTERVAL (1000000 / 360)

# include <ESP32TimerInterrupt.hpp>

#ifdef SENDECG_FIREBASE
CardiollClient cardioll;
#endif
Cardioll ecg;

ESP32Timer ITimer0(0);
bool ecgReady = true;
unsigned long process_start = 0;
unsigned long process_time = 0;
unsigned long send_time = millis(); 


bool IRAM_ATTR TimerHandler(void *timeNo)
{
    float sample = analogRead(INPUT_PIN) * 3.3 / 4095.0;

    ecg.update(sample);

    return true; 
}

void setup() {
    Serial.begin(9600);
    while (!Serial) 
        ;

#ifdef SENDECG_FIREBASE
    cardioll.setup();
#endif
    ecg.setup();

// Setup Interrupt 

    if (ITimer0.attachInterruptInterval(SAMPLE_INTERVAL, TimerHandler))
    {
        Serial.println(F("Interrupt To sample ecg attached successfully"));
    } else
        Serial.print(F("Can't set ITimer correctly. Select another frequency"));
    
    Serial.print("Initial free heap: ");
    Serial.println(ESP.getFreeHeap());

    Serial.flush();
}


void loop() {
    cardioll.authHandler();
    cardioll.Docs.loop();
#ifdef SENDECG_FIREBASE
    cardioll.loop(ecg);
    ecgReady = cardioll.patentSynced && !cardioll.sendEcg; 
#endif
    digitalWrite(LED_OUT, LOW);
    if (digitalRead(LO_NEG) != 1 && digitalRead(LO_POS) != 1)
    {
        digitalWrite(LED_OUT, HIGH);
        process_start = micros(); 
        ecg.processEcg();
        process_time = micros() - process_start;
    } else 
    {
        digitalWrite(LED_OUT, LOW);
    }

    if (ecgReady && ecg.IsRpeakDetected() && (millis() - send_time) >= SENDTIME) {
        send_time = millis();
#ifdef SENDECG_FIREBASE
        cardioll.sendEcg = true;
#endif

    }

#ifdef DEBUG_SERIAL
    static unsigned long last_timing_log = 0;  

    if (millis() - last_timing_log > 5000) {
        Serial.print("Processing time (us): ");
        Serial.println(process_time);

        Serial.print("Free heap: ");
        Serial.println(ESP.getFreeHeap());
        last_timing_log = millis();
    }


#endif
}