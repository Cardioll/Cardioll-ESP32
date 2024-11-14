# include "cardiollTClient.h"


/**
 * intializeCardiollClient - intialize the firebase app 
 */
bool CardiollClient::initializeCardiollClient() {
    UserAuth user_auth(API_KEY, USER_EMAIL, USER_PASSWORD, 3000);

    Serial.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);

    ssl_client.setInsecure();

    if (!verifyUser(API_KEY, USER_EMAIL, USER_PASSWORD)) {
        Serial.println("User verification failed");
        return false;
    }

    Serial.println("Initializing app...");
    initializeApp(aClient, app, getAuth(user_auth), aResult);
    authHandler();
    app.getApp<Firestore::Documents>(Docs);
    aClient.setAsyncResult(aResult);

    return true;
}