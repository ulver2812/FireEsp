#include <ESP8266WiFi.h>
#include <FireEsp.h>  // Include the FireESP library

// Wi-Fi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Firebase credentials
const char* apiKey = "your_API_KEY";
const char* authDomain = "your_AUTH_DOMAIN";
const char* databaseURL = "your_DATABASE_URL";

// Create FbServer and FbDatabase objects
FbServer server(apiKey, authDomain, databaseURL);
FbDatabase database(server);

// Example ID token (this will be retrieved after successful sign-in)
String idToken = "";

// Setup function to initialize Wi-Fi and Firebase
void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to Wi-Fi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Initialize Firebase server
  server.initialize();

  // Example Sign-In to get the idToken
  String email = "user@example.com";
  String password = "password123";
  if (auth.signIn(email, password)) {
    idToken = auth.getIdToken();
    Serial.println("Sign In successful!");
  } else {
    Serial.println("Sign In failed!");
  }

  // Example of adding data to the Firebase Realtime Database
  String path = "users/user1";
  String key = "name";
  String value = "John Doe";

  // Pass the idToken only if Firebase security rules require authentication
  if (idToken != "") {
    if (database.put(path, key, value, idToken)) {
      Serial.println("Data added successfully with authentication!");
    } else {
      Serial.println("Failed to add data with authentication.");
    }
  } else {
    if (database.put(path, key, value)) {
      Serial.println("Data added successfully (no authentication required in test mode)!");
    } else {
      Serial.println("Failed to add data.");
    }
  }

  // Example of updating data in the Firebase Realtime Database
  key = "name";
  value = "Jane Doe";

  // Update with or without idToken depending on authentication status
  if (idToken != "") {
    if (database.update(path, key, value, idToken)) {
      Serial.println("Data updated successfully with authentication!");
    } else {
      Serial.println("Failed to update data with authentication.");
    }
  } else {
    if (database.update(path, key, value)) {
      Serial.println("Data updated successfully (no authentication required in test mode)!");
    } else {
      Serial.println("Failed to update data.");
    }
  }

  // Example of getting data from the Firebase Realtime Database
  String data = database.get(path, idToken);
  Serial.print("Data fetched: ");
  Serial.println(data);

  // Example of removing data from the Firebase Realtime Database
  if (idToken != "") {
    if (database.remove(path, idToken)) {
      Serial.println("Data removed successfully with authentication!");
    } else {
      Serial.println("Failed to remove data with authentication.");
    }
  } else {
    if (database.remove(path)) {
      Serial.println("Data removed successfully (no authentication required in test mode)!");
    } else {
      Serial.println("Failed to remove data.");
    }
  }
}

void loop() {
  // Your main code loop goes here
}
