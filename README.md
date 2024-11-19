# FireESP - A C++ Firebase Library for Arduino

FireESP is a lightweight C++ library that simplifies the integration of Firebase services into your Arduino projects. It provides classes to interact with Firebase Authentication, Realtime Database, and Server Configuration. This library is designed for ease of use and aims to make Firebase integration seamless for IoT and embedded projects using Arduino-compatible boards.

## Features

- **Firebase Authentication**: Sign up, sign in, reset passwords, verify email addresses, and delete users.
- **Firebase Realtime Database**: Perform basic CRUD operations (create, read, update, delete) on Firebase's Realtime Database.
- **Server Configuration**: Set and manage Firebase project details such as API key, auth domain, and database URL.
- **HTTP Requests**: Send HTTP requests to Firebase's REST API to perform authentication and database operations.

## Requirements

- **Arduino IDE**: To compile and upload code to your Arduino or compatible board.
- **Firebase Project**: You need a Firebase project with Authentication and Realtime Database enabled.
- **Arduino-Compatible Board**: This library is designed for use with any board that supports the Arduino IDE (e.g., ESP8266, ESP32).

## Installation

### Step 1: Download or Clone the Repository
You can download or clone the FireESP library to your local machine:

```bash
git clone https://github.com/yourusername/FireESP.git
```

### Step 2: Add the Library to Arduino IDE
1. Open the Arduino IDE.
2. Go to **Sketch** > **Include Library** > **Add .ZIP Library...**.
3. Select the `FireESP` folder you just downloaded or cloned.

### Step 3: Include the Library in Your Sketch

In your Arduino sketch, include the necessary headers:

```cpp
#include <FireESP.h>
#include <FbServer.h>
#include <FbAuthentication.h>
#include <FbDatabase.h>
```

## Usage

### Firebase Configuration
Before using the library, you need to set up the Firebase server configuration with your project details. Replace the placeholders with your actual Firebase API key, authentication domain, and database URL.

```cpp
FbServer server("YOUR_API_KEY", "YOUR_AUTH_DOMAIN", "YOUR_DATABASE_URL");
server.initialize();
```

### Firebase Authentication

#### Sign Up
```cpp
FbAuthentication auth(server);
bool success = auth.signUp("user@example.com", "password123");
```

#### Sign In
```cpp
bool success = auth.signIn("user@example.com", "password123");
```

#### Reset Password
```cpp
bool success = auth.resetPassword("user@example.com");
```

#### Verify Email
```cpp
bool success = auth.verifyEmail(auth.getIdToken());
```

#### Delete User
```cpp
bool success = auth.deleteUser(auth.getIdToken());
```

### Firebase Database Operations

#### Put Data (String)
```cpp
FbDatabase database(server);
bool success = database.put("/path/to/data", "key", "value", auth.getIdToken());
```

#### Put Data (Integer)
```cpp
bool success = database.put("/path/to/data", "key", 123, auth.getIdToken());
```

#### Update Data (String)
```cpp
bool success = database.update("/path/to/data", "key", "new_value", auth.getIdToken());
```

#### Get Data
```cpp
String value = database.get("/path/to/data", auth.getIdToken());
```

#### Remove Data
```cpp
bool success = database.remove("/path/to/data", auth.getIdToken());
```

## Example

Here's an example Arduino sketch that demonstrates how to use the FireESP library:

```cpp
#include <FireESP.h>
#include <FbServer.h>
#include <FbAuthentication.h>
#include <FbDatabase.h>

FbServer server("YOUR_API_KEY", "YOUR_AUTH_DOMAIN", "YOUR_DATABASE_URL");
FbAuthentication auth(server);
FbDatabase database(server);

void setup() {
  Serial.begin(115200);
  
  server.initialize();
  
  if (auth.signUp("user@example.com", "password123")) {
    Serial.println("User signed up!");
  }
  
  if (auth.signIn("user@example.com", "password123")) {
    Serial.println("User signed in!");
  }

  database.put("/users/user1", "name", "John Doe", auth.getIdToken());
}

void loop() {
  // Your code here
}
```

## License

This library is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

## Contributing

Contributions are welcome! Please fork the repository, create a new branch, and submit a pull request with your changes.

### Guidelines
- Follow the coding style of the existing codebase.
- Ensure that your changes do not break existing functionality.
- Write tests if applicable and ensure they pass.

## Acknowledgments

- This library is built using the Arduino framework and Firebase's REST API.
- Thanks to the Firebase team for providing excellent services that make it easy to integrate authentication and databases into projects.

## Contact

For any issues or suggestions, please open an issue on the GitHub repository, or contact me directly at [developers.init.io@gmail.com].

```

### Key Sections Explained:

- **Project Description**: A brief explanation of what the library does and its features.
- **Installation**: Instructions on how to install the library into the Arduino IDE.
- **Usage**: Example code demonstrating how to use the library for Firebase authentication and database operations.
- **Example**: A simple Arduino sketch that demonstrates the usage of the library.
- **License**: Specifies that the library is open-source and licensed under the MIT License.
- **Contributing**: Guidelines for others who might want to contribute to your project.
