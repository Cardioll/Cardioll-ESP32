# include  <Arduino.h>
# include "cardiollTClient.h"

bool CardiollClient::verifyUser(const String& apiKey, const String& email, const String& password) 
{
    if (ssl_client.connected()) 
        ssl_client.stop(); 

    String host = "identitytoolkit.googleapis.com";
    bool ret = false;
    const int WAIT_TILL_AVAILABLE = 5000;
    const int PORT = 443;

    if (ssl_client.connect(host.c_str(), PORT) > 0) 
    {
        String payload = "{\"email\": \""; 
        payload += email; 
        payload += "\", \"password\": \""; 
        payload += password; 
        payload += "\", \"returnSecureToken\": true }";

        String header = "POST /v1/accounts:signInWithPassword?key=";
        header += apiKey;
        header += " HTTP/1.1\r\n";
        header += "Host: "; 
        header += host;
        header += "\r\n";
        header += "Content-Type: application/json\r\n";
        header += "Content-Length: "; 
        header += payload.length();
        header += "\r\n\r\n";

        if (ssl_client.print(header) == header.length())
        {
            if (ssl_client.print(payload) == payload.length()) 
            {
                unsigned long ms = millis(); 

                while (ssl_client.connected() && ssl_client.available() == 0 && (millis() - ms) < WAIT_TILL_AVAILABLE) 
                {
                    delay(1);
                }
                ms = millis();

                while (ssl_client.connected() && ssl_client.available() && millis() - ms < 5000) 
                {
                    String line = ssl_client.readStringUntil('\n'); 
                    
                    if (line.length()) {
                        ret = line.indexOf("HTTP/1.1 200") > -1; 
                        break;
                    }
                }
            }
        }
    }
    return (ret); 
}