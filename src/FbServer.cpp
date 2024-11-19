#include "FbServer.h"
#include <Arduino.h>  // Include Arduino library for Serial

// Constructor definition
FbServer::FbServer(String apiKey, String authDomain, String databaseURL)
    : _apiKey(apiKey), _authDomain(authDomain), _databaseURL(databaseURL) {}

// Initialize method definition
void FbServer::initialize() {
    Serial.println("Initializing Firebase server...");
    Serial.println("API Key: " + _apiKey);
    Serial.println("Auth Domain: " + _authDomain);
    Serial.println("Database URL: " + _databaseURL);
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
