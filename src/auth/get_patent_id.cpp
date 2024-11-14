# include  "cardiollTClient.h"

/**
 * set patentId field
 */
bool CardiollClient::getPatentId(String &documentPath, FirebaseJsonData &result)
{
    String data = getDocument(documentPath, PATIENT_FIELD);

    if (json.setJsonData(data)) 
    {
        json.get(result, "fields/assignedPatients/stringValue");

        if (result.success)
        {
            patentId = result.stringValue;
        }
        else 
        {
            json.clear();
            return false;
        }

        json.get(result, "fields/status/stringValue"); 

        if (result.success && result.stringValue == "assigned")
        {
            isAssigned = true; 
        } else 
        {
            isAssigned = false; 
            json.clear();
            return false;
        }

        json.clear();
    }

    return true;
}