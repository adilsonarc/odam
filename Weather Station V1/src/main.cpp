#include <Arduino.h>
#include <ESP8266WiFi.h>      // Include the Wi-Fi library
#include <ESP8266WiFiMulti.h> // Include the Wi-Fi-Multi library
#include <ESP8266WiFi.h>      // Include the Wi-Fi library
#include <ESP8266mDNS.h>      // Include the mDNS library
#include <WiFiClient.h>       // Include the WiFiClient library
#include <ESP8266WebServer.h> // Include the WebServer library

// put function declarations here:
void startSerialCommunication();
void establishingWiFiConnection();
void startMulticastDNS();
void startAccessPointMode();
void startWebServer();
void handleRoot(); // function prototypes for HTTP handlers
void handleNotFound();
void handleGetData();

ESP8266WebServer server(80); // Create a webserver object that listens for HTTP request on port 80

void setup()
{
  // put your setup code here, to run once:
  startSerialCommunication();
  establishingWiFiConnection();
  startMulticastDNS();
  startAccessPointMode();
  startWebServer();
}

void loop()
{
  // put your main code here, to run repeatedly:
  server.handleClient(); // Listen for HTTP requests from clients
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
  wifiMulti.addAP("Vodafone-6EE702", "g7Sk9VhVvs2WMNbB");

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

  Serial.println();
}

void startMulticastDNS()
{
  if (MDNS.begin("esp8266"))
  { // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
    Serial.print("URL address:\t");
    Serial.println("esp8266.local"); // Send the URL address of the ESP8266 to the computer
  }
  else
  {
    Serial.println("Error setting up MDNS responder!");
  }

  Serial.println();
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

  Serial.println();
}

void startWebServer()
{
  server.on("/", handleRoot);                  // Call the 'handleRoot' function when a client requests URI "/"
  server.onNotFound(handleNotFound);           // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  server.on("/data", HTTP_GET, handleGetData); // Call the 'handleGetData' function when a GET request is made to URI "/data"

  server.begin(); // Actually start the server
  Serial.println("HTTP server started");
}

void handleRoot()
{
  server.send(200, "text/plain", "Hello world!"); // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handleNotFound()
{
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void handleGetData()
{
// Providing a seed value to generate random number
	srand((unsigned) time(NULL));

  String clientResponse = "{";
  clientResponse += "\"temperature\":";
  clientResponse += rand();
  clientResponse += ",\"humidity\":";
  clientResponse += rand();
  clientResponse += ",\"pressure\":";
  clientResponse += rand();
  clientResponse += ",\"altitudex\":";
  clientResponse += rand();
  clientResponse += "}";

  server.send(200, "application/json", clientResponse);
}