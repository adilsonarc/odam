#include <Arduino.h>
#include <ESP8266WiFi.h>

// put function declarations here:
void configureMonitor(int);
void configureWifiAccessPoint(IPAddress, IPAddress, IPAddress, char *);
void configureWifiAccessPoint(IPAddress, IPAddress, IPAddress, char *, char *);
void configureWifiAccessPoint(IPAddress, IPAddress, IPAddress, char *, char *, int, int, int);

void monitorAccessPoint();

// put constants and variables here:
int loop_delay_time = 500; // miliseconds
int monitor_speed = 115200;

IPAddress local_IP(192, 168, 4, 22);
IPAddress gateway(192, 168, 4, 9);
IPAddress subnet(255, 255, 255, 0);

char *ssid = "AC-ESP8266";
char *password = "987654321";

// put function definitions here:
void setup()
{
  // put your setup code here, to run once:
  configureMonitor(monitor_speed);
  configureWifiAccessPoint(local_IP, gateway, subnet, ssid, password);
}

void loop()
{
  // put your main code here, to run repeatedly:
  monitorAccessPoint();

  delay(loop_delay_time);
}

// put configuration functions here:
void configureMonitor(int monitor_speed)
{
  Serial.begin(monitor_speed);
  Serial.println();
}

void configureWifiAccessPoint(IPAddress local_IP, IPAddress gateway, IPAddress subnet, char *ssid, char *password)
{
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  boolean isReady = WiFi.softAP(ssid, password);

  Serial.println(isReady ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
}

// put monitor functions here:
void monitorAccessPoint() {
  Serial.println("Soft-AP IP address = " + WiFi.softAPIP().toString());
}
