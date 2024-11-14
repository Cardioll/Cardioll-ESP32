# include "cardiollTClient.h"


/**
 * printError - print error message in a formatted format
 */
void CardiollClient::printError(int code, const String &msg)
{
    Serial.printf("Error, msg: %s, code: %d\n", msg.c_str(), code);
}