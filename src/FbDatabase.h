#ifndef FBDATABASE_H
#define FBDATABASE_H

#include <Arduino.h>
#include "FbServer.h"
#include <ArduinoJson.h>

class FbDatabase {
  public:
    FbDatabase(FbServer& server);  // Pass FbServer reference
    bool put(String path, String key, JsonDocument jsonDoc, String idToken = "");
    bool put(String path, String key, String value, String idToken = "");
    bool put(String path, String key, int value, String idToken = "");
    bool update(String path, String key, JsonDocument value, String idToken = "");
    bool update(String path, String key, String value, String idToken = "");
    bool update(String path, String key, int value, String idToken = "");
    String get(String path, String idToken = "");
    bool remove(String path, String idToken = "");

  private:
    FbServer& server;  // Store reference to the FbServer instance
    String httpRequest(String method, String url, String payload);
};

#endif
