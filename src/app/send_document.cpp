#include "cardiollTClient.h"


/**
 * sendDocument - sendDocument to firestore document 
 * @documentPath: The database path to send the document
 * @doc: The document to send to firestore document
 */
String CardiollClient::sendDocument(const String& documentPath, Document<Values::Value>& doc) {
    String payload = Docs.createDocument(aClient, Firestore::Parent(PROJECT_ID, DATABASE_ID), documentPath, DocumentMask(), doc);

    if (aClient.lastError().code() == 0) {
        Serial.println("Document sent successfully");
        Serial.println(payload);
    } else {
        printError(aClient.lastError().code(), aClient.lastError().message());
    }

    return payload;
}
