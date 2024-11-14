#include "cardiollTClient.h"


/**
 * authHandler - handles authentication to firebase
 */
void CardiollClient::authHandler() {
    unsigned long start_time = millis();
    const unsigned long timeout = 120 * 1000;  // 120 seconds timeout

    while (app.isInitialized() && !app.ready() 
    && (millis() - start_time) < timeout) {
        
        JWT.loop(app.getAuth());
        printResult();
    }
}