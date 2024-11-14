#ifndef CARDIOLL_T_CLIENT_H
#define CARDIOLL_T_CLIENT_H

#include <Arduino.h>
#include <FirebaseClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <FirebaseJson.h>
#include "Cardioll.h"

#define WIFI_SSID "MTN_4G_3160DE"
#define WIFI_PASSWORD "BE2EAFD5"
#define USER_EMAIL "cardioll12@gmail.com"
#define USER_PASSWORD "cardioll12"
#define API_KEY "AIzaSyBUPodIVTbSG0R_E51mis5xD4Zz3B0OP1g"
#define PROJECT_ID "cardioll"
#define DATABASE_ID ""
#define DEVICE_ID "ECG_EqYSnZWpffyccCjaVN7W"
#define DEVICE_COL "devices"
#define PATIENT_FIELD "assignedPatients,status"
#define PATIENT_COLUMN "patients"
#define ECG_COLUMN "processedECG"
#define ALERT_COLUMN "alerts"
#define DEBUG true
#define SENDTIME (1000 * 10)

/**
 * @brief CardiollClient class handles interactions with Firebase.
 */
class CardiollClient {
 private:
   struct PatentInfo{
      int age; 
      int sex;
   };
    PatentInfo patent_info;
    FirebaseApp app;
    bool taskComplete;
    AsyncResult aResult;
    DefaultNetwork network;
    WiFiClientSecure ssl_client;
    AsyncClientClass aClient;
    unsigned long dataMillis;
    unsigned long lastLoopCheck;
    bool isAssigned;
    FirebaseJson json;
    unsigned long checkIfPatentChange;

    bool verifyUser(const String& apiKey, const String& email, const String& password);
    bool initializeCardiollClient();
    const String detect_trachidia(int age, int sex, float HR, float PR, float QRS, float QT, float QTC, float RR);
    bool getPatentInfo(const String documentPath, const String &mask, FirebaseJsonData &json);
    
 public:
    String patentId;
    bool sendEcg;
    bool patentSynced;
    Firestore::Documents Docs;

    CardiollClient();
    void setup();
    void loop(Cardioll &ecg);
    void printResult();
    void authHandler();
    String sendDocument(const String& documentPath, Document<Values::Value>& doc); 
    void printError(int code, const String& msg);
    String getDocument(const String& documentPath, const String& mask);
    uint32_t getTimestamp();
    bool getPatentId(String &documentPath, FirebaseJsonData &result);
    void syncPatentId(void);
    void createDocument(Cardioll &ecg, Document<Values::Value> &doc, Document<Values::Value> &alert);
    bool patchDocument(const String& documentPath, PatchDocumentOptions &patchOptions, Document<Values::Value>& doc);
};

#endif