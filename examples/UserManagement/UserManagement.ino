#include <ESP8266WiFi.h>
#include <FireEsp.h>  // Include the FireESP library

// Wi-Fi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Firebase credentials
const char* apiKey = "your_API_KEY";
const char* authDomain = "your_AUTH_DOMAIN";
const char* databaseURL = "your_DATABASE_URL";

// Create FbServer and FbAuthentication objects
FbServer server(apiKey, authDomain, databaseURL);
FbAuthentication auth(server);

// Setup function to initialize the Wi-Fi and Firebase
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

  // Example of SignUp
  String email = "user@example.com";
  String password = "password123";
  if (auth.signUp(email, password)) {
    Serial.println("Sign Up successful!");
  } else {
    Serial.println("Sign Up failed!");
  }

  // Example of SignIn
  if (auth.signIn(email, password)) {
    Serial.println("Sign In successful!");
    Serial.print("User ID: ");
    Serial.println(auth.getUserId());
  } else {
    Serial.println("Sign In failed!");
  }

  // Example of Reset Password
  if (auth.resetPassword(email)) {
    Serial.println("Password reset email sent!");
  } else {
    Serial.println("Password reset failed!");
  }

  // Example of Email Verification
  if (auth.verifyEmail(auth.getIdToken())) {
    Serial.println("Email verification sent!");
  } else {
    Serial.println("Email verification failed!");
  }

  // Example of Email Verification Check
  if (auth.checkEmailVerified(auth.getIdToken())) {
    Serial.println("Email verified!");
  } else {
    Serial.println("Email not verified!");
  }
}

void loop() {
  // Your main code loop goes here
}
