#include "cardiollTClient.h"

CardiollClient::CardiollClient() 
    : taskComplete(false), 
      aClient(ssl_client, getNetwork(network)),
      dataMillis(0),
      lastLoopCheck(0),
      isAssigned(false),
      patentId(""),
      sendEcg(false),
      patentSynced(false){
}

void CardiollClient::setup() {
    Serial.begin(9600);
    Serial.println("\nStarting CardiollClient setup...");

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        if (WiFi.status() == WL_NO_SHIELD)
            Serial.println("No wifi shield");
        else if(WiFi.status() == WL_CONNECTION_LOST)
            Serial.println("Couldn't connect");
        else if(WiFi.status() == WL_NO_SSID_AVAIL)
            Serial.println("AccessPoint is not available");
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP().toString());

    if (initializeCardiollClient()) {
        Serial.println("CardiollClient initialized successfully");
    } else {
        Serial.println("Failed to initialize CardiollClient");
    }
}
