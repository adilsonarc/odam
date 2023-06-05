#include <Arduino.h>
#include <ESP8266WiFi.h>      // Include the Wi-Fi library
#include <ESP8266WiFiMulti.h> // Include the Wi-Fi-Multi library
#include <ESP8266WiFi.h>      // Include the Wi-Fi library

// put function declarations here:
void startSerialCommunication();
void establishingWiFiConnection();
void startAccessPointMode();

void setup()
{
  // put your setup code here, to run once:
  startSerialCommunication();
  establishingWiFiConnection();
  startAccessPointMode();
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
  ESP8266WiFiMulti wifiMulti; // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

  // Add multipe network
  // It will connect to the strongest of the networks
  wifiMulti.addAP("ssid_from_AP_1", "your_password_for_AP_1"); // add Wi-Fi networks you want to connect to
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED)
  { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(1000);
    Serial.print(++i);
    Serial.print(' ');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID()); // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP()); // Send the IP address of the ESP8266 to the computer
}

void startAccessPointMode()
{
  const char *ssid = "ESP8266 Access Point"; // The name of the Wi-Fi network that will be created
  const char *password = "987654321";        // The password required to connect to it, leave blank for an open network

  WiFi.softAP(ssid, password); // Start the access point
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP()); // Send the IP address of the ESP8266 to the computer
}