# include "cardiollTClient.h"

/**
 * getTimes#include "cardiollTClient.h"

/**
 * getTimestamp: get time stamp 
 * Return: time stamp or 0 if failed
 */
uint32_t CardiollClient::getTimestamp() {
    const int MAX_RETRIES = 5;
    const int RETRY_DELAY_MS = 1000;

    if (time(nullptr) < FIREBASE_DEFAULT_TS) {
        configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
        
        for (int i = 0; i < MAX_RETRIES; i++) {
#ifdef DEBUG_SERIAL
            Serial.println("Attempting to sync time...");
#endif
            
            int timeout = 5000; // 5 seconds timeout
            while (time(nullptr) < FIREBASE_DEFAULT_TS && timeout > 0) {
                delay(100);
                timeout -= 100;
                yield();  // Allow system tasks to run
            }
            
            if (time(nullptr) >= FIREBASE_DEFAULT_TS) {
#ifdef DEBUG_SERIAL
                Serial.println("Time synced successfully");
#endif
                return time(nullptr);
            }
#ifdef DEBUG_SERIAL            
            Serial.println("Time sync failed, retrying...");
#endif
            delay(RETRY_DELAY_MS);
        }

#ifdef DEBUG_SERIAL      
        Serial.println("Failed to sync time after maximum retries");
#endif
        return 0; // Indicate failure
    }
    
    return time(nullptr);
}