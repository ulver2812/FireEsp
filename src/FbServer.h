#ifndef FBSERVER_H
#define FBSERVER_H

#include <Arduino.h>

class FbServer {
public:
    // Constructor
    FbServer(String apiKey, String authDomain, String databaseURL);

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
};

#endif // FBSERVER_H
