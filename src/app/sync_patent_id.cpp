#include "cardiollTClient.h"


/**
 * sync patent id from database and reflect changes as they happen
 * 
 */
void CardiollClient::syncPatentId(void)
{
    String documentPath = DEVICE_COL "/" DEVICE_ID;
    FirebaseJsonData result;
    PatchDocumentOptions patchOptions(DocumentMask("lastSyncTime"), DocumentMask(), Precondition());

    if (this->getPatentId(documentPath, result))
    {
        Values::DoubleValue timeT(getTimestamp());
        String mask = "gender,age";

        Document<Values::Value> doc("lastSyncTime", Values::Value(timeT));
        patchDocument(documentPath, patchOptions, doc);
        checkIfPatentChange = millis();
        documentPath = PATIENT_COLUMN "/"; 
        documentPath += patentId; 

        getPatentInfo(documentPath, mask, result);
    }
}