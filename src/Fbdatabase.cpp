#include "FbDatabase.h"
#include <WiFiClientSecure.h>

// Constructor
FbDatabase::FbDatabase(FbServer& serverRef) : server(serverRef) {}

// Put (String value)
bool FbDatabase::put(String path, String key, String value, String idToken) {
    String url = server.getDatabaseURL() + (path.startsWith("/") ? path : "/" + path) + ".json" +
                 (idToken.length() > 0 ? "?auth=" + idToken : "");
    String payload = "{\"" + key + "\":\"" + value + "\"}";
    String response = httpRequest("PATCH", url, payload);

    Serial.println("PUT Response (String): " + response);
    return response.indexOf("error") == -1;
}

// Put (int value)
bool FbDatabase::put(String path, String key, int value, String idToken) {
    String url = server.getDatabaseURL() + (path.startsWith("/") ? path : "/" + path) + ".json" +
                 (idToken.length() > 0 ? "?auth=" + idToken : "");
    String payload = "{\"" + key + "\":" + String(value) + "}";
    String response = httpRequest("PATCH", url, payload);

    Serial.println("PUT Response (int): " + response);
    return response.indexOf("error") == -1;
}

// Update (String value) - Alias for put
bool FbDatabase::update(String path, String key, String value, String idToken) {
    return put(path, key, value, idToken);
}

// Update (int value) - Alias for put
bool FbDatabase::update(String path, String key, int value, String idToken) {
    return put(path, key, value, idToken);
}

// Get data from the database
String FbDatabase::get(String path, String idToken) {
    String url = server.getDatabaseURL() + (path.startsWith("/") ? path : "/" + path) + ".json" +
                 (idToken.length() > 0 ? "?auth=" + idToken : "");
    String response = httpRequest("GET", url, "");

    Serial.println("GET Response: " + response);
    return response;
}

// Remove data from the database
bool FbDatabase::remove(String path, String idToken) {
    String url = server.getDatabaseURL() + (path.startsWith("/") ? path : "/" + path) + ".json" +
                 (idToken.length() > 0 ? "?auth=" + idToken : "");
    String response = httpRequest("DELETE", url, "");

    Serial.println("DELETE Response: " + response);
    return response.indexOf("error") == -1;
}

// Helper method to make HTTP requests
String FbDatabase::httpRequest(String method, String url, String payload) {
    WiFiClientSecure client;
    client.setInsecure(); // Disable certificate validation (not recommended for production)

    // Extract the host and path from the URL
    String host = url.substring(8, url.indexOf('/', 8)); // Extract host after "https://"
    String path = url.substring(url.indexOf('/', 8));    // Extract path

    if (!client.connect(host.c_str(), 443)) {
        return "{\"error\": \"Connection failed to " + host + "\"}";
    }

    // Build the HTTP request
    String request = method + " " + path + " HTTP/1.1\r\n";
    request += "Host: " + host + "\r\n";
    request += "Content-Type: application/json\r\n";
    if (payload.length() > 0) {
        request += "Content-Length: " + String(payload.length()) + "\r\n";
    }
    request += "Connection: close\r\n\r\n";
    if (payload.length() > 0) {
        request += payload;
    }

    // Send the request
    client.print(request);

    // Wait for a response
    while (!client.available()) {
        delay(10);
    }

    // Read the status line (e.g., "HTTP/1.1 200 OK")
    String statusLine = client.readStringUntil('\n');
    Serial.println("HTTP Status Line: " + statusLine);

    // Extract the HTTP status code
    int httpCode = statusLine.substring(9, 12).toInt();
    if (httpCode >= 400) {
        String errorMsg = "HTTP Error: " + String(httpCode);
        String responseBody = client.readString();
        if (responseBody.length() > 0) {
            errorMsg += "\nResponse Body: " + responseBody;
        }
        client.stop();
        return "{\"error\": \"" + errorMsg + "\"}";
    }

    // Read the rest of the response
    String response = client.readString();
    client.stop();

    // Return the response
    return response;
}
