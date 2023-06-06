#include <Arduino.h>
#include <ESP8266WiFi.h>      // Include the Wi-Fi library
#include <ESP8266WiFiMulti.h> // Include the Wi-Fi-Multi library
#include <ESP8266WiFi.h>      // Include the Wi-Fi library
#include <ESP8266mDNS.h>      // Include the mDNS library
#include <WiFiClient.h>       // Include the WiFiClient library
#include <ESP8266WebServer.h> // Include the WebServer library
#include "LittleFS.h"         // Include the WebServer library
#include "BME280.h"           // Include the BME280 sensor library

// put function declarations here:
void startSerialCommunication();
void startFileSystem();
void establishingWiFiConnection();
void startMulticastDNS();
void startAccessPointMode();
void startSensor();
float getVoltage();
float getPressure();
float getAltiture();
float getHumidity();
float getTemperature();
void startWebServer();
void handleRoot(); // function prototypes for HTTP handlers
void handleNotFound();
void handleGetMetrics();

ESP8266WebServer server(80); // Create a webserver object that listens for HTTP request on port 80
BME280 bme280;               // BME280 sensor

void setup()
{
  // put your setup code here, to run once:
  startSerialCommunication();
  startFileSystem();
  // establishingWiFiConnection();
  // startMulticastDNS();
  startAccessPointMode();
  startSensor();
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

  Serial.println("\nDebug v2");

  Serial.println('\n');
}

void startFileSystem()
{
  if (!LittleFS.begin())
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
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

  Serial.println();
}

void startMulticastDNS()
{
  if (MDNS.begin("odam"))
  { // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
    Serial.print("URL address:\t");
    Serial.println("odam.local"); // Send the URL address of the ESP8266 to the computer
  }
  else
  {
    Serial.println("Error setting up MDNS responder!");
  }

  Serial.println();
}

void startAccessPointMode()
{
  const char *ssid = "ODAM ESP8266 Access Point"; // The name of the Wi-Fi network that will be created
  const char *password = "987654321";             // The password required to connect to it, leave blank for an open network

  WiFi.softAP(ssid, password); // Start the access point
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP()); // Send the IP address of the ESP8266 to the computer

  Serial.println();
}

void startSensor()
{
  if (!bme280.init())
  {
    Serial.println("Device error!");
  }
}

void startWebServer()
{
  server.on("/", handleRoot);                        // Call the 'handleRoot' function when a client requests URI "/"
  server.onNotFound(handleNotFound);                 // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  server.on("/metrics", HTTP_GET, handleGetMetrics); // Call the 'handleGetMetrics' function when a GET request is made to URI "/data"

  server.begin(); // Actually start the server
  Serial.println("HTTP server started");
}

void handleRoot()
{
  Serial.println("Request for root page");
  String contentType = "text/html"; // Get the MIME type
  String rootPage = "/web-server/index.html";
  File file = LittleFS.open(rootPage, "r"); // Open the file
  if (!file)
  {
    Serial.println("Failed to open file " + rootPage + " for reading");
    return;
  }

  server.streamFile(file, "text/html");
  file.close(); // Close the file again
  Serial.println(String("Sent file: ") + rootPage);
}

void handleNotFound()
{
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void handleGetMetrics()
{
  // Providing a seed value to generate random number
  srand((unsigned)time(NULL));

  String clientResponse = "{";
  clientResponse += "\"temperature\":";
  clientResponse += rand();
  clientResponse += ",\"humidity\":";
  clientResponse += rand();
  clientResponse += ",\"pressure\":";
  clientResponse += rand();
  clientResponse += ",\"altitude\":";
  clientResponse += rand();
  clientResponse += ",\"battery\":";
  clientResponse += rand();
  clientResponse += "}";

  server.send(200, "application/json", clientResponse);
}

float getVoltage()
{
  int reading = analogRead(sensorPin);
  float voltage = reading * 3.3;
  voltage /= 1024.0;
  return voltage;
}

float getPressure()
{
  float pressure = bme280.getPressure();  // pressure in Pa
  float pressureInHPa = pressure / 100.0; // pressure in hPa
  return pressureInHPa;
}

float getAltiture()
{
  float pressure = getPressure();
  float altitude = bme280.calcAltitude(pressure);
  return altitude;
}

float getHumidity()
{
  float humidity = bme280.getHumidity();
  return humidity;
}

float getTemperature()
{
  float temperatureC = (voltage - 0.5) * 100;
  return temperatureC;
}
