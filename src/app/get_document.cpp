# include "cardiollTClient.h"


/**
 * getDocument - get document from firestore
 */
String CardiollClient::getDocument(const String& documentPath, const String& mask)
{
#ifdef DEBUG_SERIAL
    Serial.println("Getting a document");
#endif

    String payload = this->Docs.get(this->aClient, Firestore::Parent(PROJECT_ID, DATABASE_ID),
     documentPath, GetDocumentOptions(DocumentMask(mask)));

    if (this->aClient.lastError().code() == 0)
                Serial.println(payload);
            else
                printError(this->aClient.lastError().code(), 
                this->aClient.lastError().message());
    return payload;
}