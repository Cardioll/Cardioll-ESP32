# include "cardiollTClient.h"

/**
 * printResult - print the async client result to Serial monitor
 */
void CardiollClient::printResult()
{
    if (aResult.isEvent()) 
    {
        Serial.printf("Event task: %s, msg: %s, code: %d\n", 
        aResult.uid().c_str(), aResult.appEvent().message().c_str(), 
        aResult.appEvent().code()); 
    }

    if (aResult.isDebug())
    {
        Serial.printf("Debug task: %s, msg: %s\n", 
        aResult.uid().c_str(), aResult.debug().c_str());
    }

    if (aResult.isError())
    {
        Serial.printf("Error task: %s, msg: %s, code: %d",
         aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());
    }

    if (aResult.available())
    {
        Serial.printf("task: %s, payload: %s\n", aResult.uid().c_str(), aResult.c_str());
    }
}