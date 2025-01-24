#include "FbAuthentication.h"
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

// Constructor
FbAuthentication::FbAuthentication(FbServer& serverRef) : server(serverRef) {}  // Initialize the reference

// Helper method to perform HTTP requests
String FbAuthentication::httpRequest(String method, String url, String payload) {
    WiFiClientSecure client;
    client.setInsecure();  // Ignore SSL/TLS certificate validation for testing

    // Extract host from URL
    String host = "identitytoolkit.googleapis.com";

    if (!client.connect(host.c_str(), 443)) {
        return "{\"error\": \"Connection failed\"}";
    }

    // Build HTTP request
    String request = method + " " + url.substring(url.indexOf("/", 8)) + " HTTP/1.1\r\n";
    request += "Host: " + host + "\r\n";
    request += "Content-Type: application/json\r\n";
    request += "Content-Length: " + String(payload.length()) + "\r\n";
    request += "Connection: close\r\n\r\n";
    request += payload;

    // Send HTTP request
    client.print(request);

    // Wait for response with timeout (1 second delay is too rigid)
    unsigned long startMillis = millis();
    unsigned long timeout = 10000;  // Timeout set to 10 seconds (adjustable)

    while (client.connected() && !client.available()) {
        if (millis() - startMillis > timeout) {
            // Timeout occurred, break out of the loop
            return "{\"error\": \"Timeout waiting for response\"}";
        }
        delay(10);  // Slight delay to avoid overloading the CPU
    }

    // Read the response body
    String responseBody = "";
    while (client.available()) {
        String line = client.readStringUntil('\n');
        responseBody += line;
    }

    // Return the full response
    return responseBody;
}

// Helper method to extract tokens from the response
String FbAuthentication::extractTokenFromResponse(String response, String tokenName) {
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, response);

    if (error) {
        return "";
    }

    if (doc.containsKey(tokenName)) {
        return doc[tokenName].as<String>();
    }

    return "";
}

// Helper method to extract detailed error message from response
String FbAuthentication::getErrorMessage(String response) {
    int errorPos = response.indexOf("\"error\"");
    if (errorPos != -1) {
        int messagePos = response.indexOf("\"message\":", errorPos);
        if (messagePos != -1) {
            String errorMessage = response.substring(messagePos + 11);  // skip the `"message":`
            errorMessage = errorMessage.substring(0, errorMessage.indexOf("\""));
            return errorMessage;
        }
    }
    return "Unknown error";  // Default error if no message is found
}

// Method to sign up a new user
bool FbAuthentication::signUp(String email, String password) {
    String url = "https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=" + server.getApiKey();
    String payload = "{\"email\":\"" + email + "\",\"password\":\"" + password + "\",\"returnSecureToken\":true}";
    String response = httpRequest("POST", url, payload);

    // Parse the JSON response
    StaticJsonDocument<1024> jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, response);

    // Handle parsing errors
    if (error) {
        Serial.println("Error parsing signUp response: " + String(error.c_str()));
        return false;
    }

    // Extract fields from the JSON response
    String idToken = jsonDoc["idToken"] | ""; // Default to empty string if not found
    String localId = jsonDoc["localId"] | ""; // Default to empty string if not found

    // Check if the idToken and localId are valid (indicating a successful sign-up)
    if (idToken != "" && localId != "") {
        this->idToken = idToken;
        this->localId = localId;
        return true;
    } else {
        Serial.println("Error: Missing idToken or localId");
        return false;
    }
}

// Method to sign in an existing user
bool FbAuthentication::signIn(String email, String password) {
    String url = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + server.getApiKey();
    String payload = "{\"email\":\"" + email + "\",\"password\":\"" + password + "\",\"returnSecureToken\":true}";
    String response = httpRequest("POST", url, payload);

    // Parse the JSON response
    StaticJsonDocument<1024> jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, response);

    // Handle parsing errors
    if (error) {
        Serial.println("Error parsing signIn response: " + String(error.c_str()));
        return false;
    }

    // Extract idToken and localId from the response
    String idToken = jsonDoc["idToken"] | "";  // Default to empty string if not found
    String localId = jsonDoc["localId"] | "";  // Default to empty string if not found

    // Check if the idToken and localId are valid (indicating a successful sign-in)
    if (idToken != "" && localId != "") {
        this->idToken = idToken;
        this->localId = localId;
        return true;
    } else {
        Serial.println("Error: Missing idToken or localId in signIn response");
        return false;
    }
}

// Method to retrieve the idToken
String FbAuthentication::getIdToken() {
    return idToken;
}

// Method to retrieve the user ID (localId)
String FbAuthentication::getUserId() {
    return localId;
}

// Method to reset the user's password
bool FbAuthentication::resetPassword(String email) {
    String url = "https://identitytoolkit.googleapis.com/v1/accounts:sendOobCode?key=" + server.getApiKey();
    String payload = "{\"requestType\":\"PASSWORD_RESET\",\"email\":\"" + email + "\"}";
    String response = httpRequest("POST", url, payload);

    // Check if response contains the "email" key to determine success
    if (response.indexOf("email") != -1) {
        return true;
    }

    // Parse error message if any
    String errorMessage = getErrorMessage(response);
    if (errorMessage != "Unknown error") {
        Serial.println("Password reset error: " + errorMessage);
    }

    return false;
}

// Method to verify the user's email
bool FbAuthentication::verifyEmail(String idToken) {
    String url = "https://identitytoolkit.googleapis.com/v1/accounts:sendOobCode?key=" + server.getApiKey();
    String payload = "{\"requestType\":\"VERIFY_EMAIL\",\"idToken\":\"" + idToken + "\"}";
    String response = httpRequest("POST", url, payload);

    // Check if response contains the "email" key to determine success
    if (response.indexOf("email") != -1) {
        return true;
    }

    // Parse error message if any
    String errorMessage = getErrorMessage(response);
    if (errorMessage != "Unknown error") {
        Serial.println("Email verification error: " + errorMessage);
    }

    return false;
}

// Method to check if email is verified
bool FbAuthentication::checkEmailVerified(String idToken) {
    String url = "https://identitytoolkit.googleapis.com/v1/accounts:lookup?key=" + server.getApiKey();
    String payload = "{\"idToken\":\"" + idToken + "\"}";
    String response = httpRequest("POST", url, payload);

    // Check if the response contains "emailVerified":true
    if (response.indexOf("\"emailVerified\":true") != -1) {
        return true;
    }

    return false;
}

// Method to delete the user
bool FbAuthentication::deleteUser(String idToken) {
    String url = "https://identitytoolkit.googleapis.com/v1/accounts:delete?key=" + server.getApiKey();
    String payload = "{\"idToken\":\"" + idToken + "\"}";
    String response = httpRequest("POST", url, payload);

    // Check if the response contains "idToken"
    if (response.indexOf("idToken") != -1) {
        return true;
    }

    return false;
}
