#include <Arduino.h>
#include <ESP8266WiFi.h>

// put function declarations here:
int configureWifiAccessPoint(String);

// put constants here:
const char *ssid = "AC-ESP8266";
const char *password = "987654321";

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

// put function definitions here:
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid,password) ? "Ready" : "Failed!");
  //WiFi.softAP(ssid);
  //WiFi.softAP(ssid, password, channel, hidden, max_connection)
  
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("[Server Connected] ");
  Serial.println(WiFi.softAPIP());

  delay(500);
}
