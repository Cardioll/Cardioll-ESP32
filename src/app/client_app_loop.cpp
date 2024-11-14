#include "cardiollTClient.h"
#define MIN_1 60 * 1000
#define MIN_5 (30 * 1000)


/**
 * @brief handles all recurrent loop activity for app
 */
void CardiollClient::loop(Cardioll &ecg) {
   if (millis() - lastLoopCheck >= 1000) 
   {  
        lastLoopCheck = millis();
        
        
        if (app.ready()) 
        {
            if (!isAssigned || !patentId.length() || 
            (isAssigned && (millis() - checkIfPatentChange) > MIN_5)) {
              syncPatentId();
              patentSynced = true;
            }

            if (isAssigned && sendEcg && patentId.length()) {
              String alertPath = PATIENT_COLUMN "/";
              String docPath = PATIENT_COLUMN "/";
              docPath += patentId;
              alertPath += patentId; 
              alertPath += "/" ALERT_COLUMN;
              docPath += "/" ECG_COLUMN;

              Values::DoubleValue timeStamp(getTimestamp());
              Document<Values::Value> doc("timestamp", Values::Value(timeStamp));
              Document<Values::Value> alert("timestamp", Values::Value(timeStamp));
              createDocument(ecg, doc, alert);
              sendDocument(docPath, doc);
              
              sendDocument(alertPath, alert);

              doc.clear();
              alert.clear();
              sendEcg = false;
            }
        }
  }
}