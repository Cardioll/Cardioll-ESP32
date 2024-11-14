# include "cardiollTClient.h"

/**
 * patchDocument - update a document in on firestore
 * @documentPath: path to the the document to update 
 * @mask:  Mask of the document to document
 * @doc: The document containing the value to update
 */
bool CardiollClient::patchDocument(const String &documentPath, PatchDocumentOptions &patchOptions, Document<Values::Value> &doc)
{
    String payload = Docs.patch(aClient, Firestore::Parent(PROJECT_ID), documentPath, patchOptions, doc);

    if (aClient.lastError().code() == 0 && DEBUG)
    {
        Serial.println(payload);
    } 
    else 
    {
        printError(aClient.lastError().code(), aClient.lastError().message());
        return false;
    }

    return true;
}