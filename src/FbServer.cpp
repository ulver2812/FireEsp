#include "FbServer.h"
#include <Arduino.h>  // Include Arduino library for Serial

// Constructor definition
FbServer::FbServer(const String& apiKey, const String& authDomain, const String& databaseURL)
    : _apiKey(apiKey), _authDomain(authDomain), _databaseURL(databaseURL) {}

// Initialize method definition
void FbServer::initialize() {
    debugPrint("Initializing Firebase server...");
    debugPrint("API Key: " + _apiKey);
    debugPrint("Auth Domain: " + _authDomain);
    debugPrint("Database URL: " + _databaseURL);
}

// Getter method for API Key
String FbServer::getApiKey() const {
    return _apiKey;
}

// Getter method for Auth Domain
String FbServer::getAuthDomain() const {
    return _authDomain;
}

// Getter method for Database URL
String FbServer::getDatabaseURL() const {
    return _databaseURL;
}

// Debug helper method
void FbServer::debugPrint(const String& message) const {
#if DEBUG // Only include this code if DEBUG is enabled
    Serial.println(message);
#endif
}
