#ifndef FBSERVER_H
#define FBSERVER_H

#include <Arduino.h>

// Define DEBUG for enabling/disabling debug prints
#define DEBUG 1 // Set to 0 for production, 1 for debugging

class FbServer {
public:
    // Constructor
    FbServer(const String& apiKey, const String& authDomain, const String& databaseURL);

    // Public Methods
    void initialize();
    
    // Getter methods to access private members
    String getApiKey() const;
    String getAuthDomain() const;
    String getDatabaseURL() const;

private:
    // Private member variables
    String _apiKey;
    String _authDomain;
    String _databaseURL;

    // Debug helper method
    void debugPrint(const String& message) const;
};

#endif // FBSERVER_H
