#include <Arduino.h>
#include <ESP8266WiFi.h> // Include the Wi-Fi library

// put function declarations here:
void startSerialCommunication();
void establishingWiFiConnection();

void setup()
{
  // put your setup code here, to run once:
  startSerialCommunication();
  establishingWiFiConnection();
}

void loop()
{
  // put your main code here, to run repeatedly:
}

// put function definitions here:

void startSerialCommunication()
{
  Serial.begin(115200); // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');
}

void establishingWiFiConnection()
{
  const char *ssid = "SSID";         // The SSID (name) of the Wi-Fi network you want to connect to
  const char *password = "PASSWORD"; // The password of the Wi-Fi network

  WiFi.begin(ssid, password); // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED)
  { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i);
    Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP()); // Send the IP address of the ESP8266 to the computer
}