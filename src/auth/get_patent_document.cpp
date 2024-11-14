# include "cardiollTClient.h"


bool CardiollClient::getPatentInfo(const String documentPath, const String &mask, FirebaseJsonData &result)
{
    String data = getDocument(documentPath, mask);
    
    if (json.setJsonData(data))
    {
        json.get(result, "fields/gender/stringValue");

        if (result.success)
        {
            patent_info.sex = (result.stringValue == "male") ? 1 : 0;
        } else 
        {
            json.clear();
            return false;
        }

        json.get(result, "fields/age/integerValue");

        if (result.success)
        {
            patent_info.age = result.intValue;
        } else 
        {
            json.clear();
            return false;
        }

        json.clear();
        return true;
    }

    return false;  
}