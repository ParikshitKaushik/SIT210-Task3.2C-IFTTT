#include <WiFiNINA.h>  // Include the WiFiNINA library for Wi-Fi communication
#include <BH1750.h>    // Include the BH1750 library for the light sensor
#include <Wire.h>      // Include the Wire library for I2C communication

// Define your Wi-Fi network credentials
char ssid[] = "Moto G";   // Your Wi-Fi network name
char pass[] = "12341234";  // Your Wi-Fi network password

WiFiClient client;  // Create a Wi-Fi client object
BH1750 lightMeter;  // Create a BH1750 light sensor object

char HOST_NAME[] = "maker.ifttt.com";  // Define the IFTTT server's hostname
String PATH_NAME = "/trigger/Light/with/key/sjanjdnundUENFONDI";  // Define the request path
String queryString = "?value1=57&value2=25";  // Initialize a query string with values

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.print("START");
  while (!Serial);  // Wait for the serial connection to be established

  // Connect to the Wi-Fi network
  WiFi.begin(ssid, pass);

  Wire.begin();  // Initialize the I2C communication

  while (true) {
    if (client.connect(HOST_NAME, 80)) {
      // If connected to the server, print a message
      Serial.println("Connected to server");
      break;
    } else {
      // If not connected to the server, print a connection failed message
      Serial.println("connection failed");
    }
    delay(500);  // Delay before retrying the connection
  }

  lightMeter.begin();  // Initialize the light sensor
  Serial.println("Connected to server");
}

void loop() {
  Serial.print("START");

  // Read the light level from the sensor
  float lux = lightMeter.readLightLevel();

  queryString += "?value1=";  // Modify the query string
  queryString += lux;         // Add the light level to the query string
  Serial.println(queryString);

  if (lux > 500) {
    // If the light level is above a threshold (500), make an HTTP request

    // Send an HTTP GET request to the IFTTT server
    client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();  // End the HTTP header

    while (client.connected()) {
      if (client.available()) {
        // Read an incoming byte from the server and print it to the serial monitor
        char c = client.read();
        Serial.print(c);
      }
    }

    // The server's disconnected, stop the client
    client.stop();
    Serial.println();
    Serial.println("disconnected");
  }

  queryString = "";  // Reset the query string
  delay(3000);  // Delay before the next loop iteration
}
