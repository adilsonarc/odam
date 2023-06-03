#include <Arduino.h>
#include <ESP8266WiFi.h>
//#include <WiFi.h>

// put function declarations here:
void configureMonitor(int);
void configureWifiAccessPoint(IPAddress, IPAddress, IPAddress, char *, char *);
void configureWebServer();

void serverRequestHandler();
char * getHTML();

// put constants and variables here:
int loop_delay_time = 500; // miliseconds
int monitor_speed = 115200;

IPAddress local_IP(192, 168, 4, 22);
IPAddress gateway(192, 168, 4, 9);
IPAddress subnet(255, 255, 255, 0);

// Network credentials
char *ssid = "AC-ESP8266";
char *password = "987654321";

WiFiServer server(80);

// Variable to store the HTTP request
String header;

// put function definitions here:
void setup()
{
  // put your setup code here, to run once:
  configureMonitor(monitor_speed);
  configureWifiAccessPoint(local_IP, gateway, subnet, ssid, password);
  configureWebServer();
}

void loop()
{
  // put your main code here, to run repeatedly:
  serverRequestHandler();
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

  Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");

Serial.println("Soft-AP IP address = " + WiFi.softAPIP().toString());
}

void configureWebServer()
{
  server.begin();
}

// put monitor functions here:
void serverRequestHandler()
{
  WiFiClient client = server.accept(); // Listen for incoming clients

  if (client)
  {                                // If a new client connects,
    Serial.println("New Client."); // print a message out in the serial port
    String currentLine = "";       // make a String to hold incoming data from the client
    while (client.connected())
    { // loop while the client's connected
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c);        // print it out the serial monitor
        header += c;
        if (c == '\n')
        { // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            client.println(getHTML());

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          }
          else
          { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

char * getHTML()
{
  return "<!DOCTYPE html>"
"<!-- Template by html.am -->"
"<html>"
"<head>"
    "    <meta http-equiv='Content-Type' content='text/html; charset=UTF-8'>"
    "    <title>3 Column Layout</title>"
    "    <style type='text/css'>"
    ""
    "		/* Layout */"
    "		body {"
    "			min-width: 630px;"
    "		}"
    ""
    "		#container {"
    "			padding-left: 200px;"
    "			padding-right: 190px;"
    "		}"
    "		"
    "		#container .column {"
    "			position: relative;"
    "			float: left;"
    "		}"
    "		"
    "		#center {"
    "			padding: 10px 20px;"
    "			width: 100%;"
    "		}"
    "		"
    "		#left {"
    "			width: 180px;"
    "			padding: 0 10px;"
    "			right: 240px;"
    "			margin-left: -100%;"
    "		}"
    "		"
    "		#right {"
    "			width: 130px;"
    "			padding: 0 10px;"
    "			margin-right: -100%;"
    "		}"
    "		"
    "		#footer {"
    "			clear: both;"
    "		}"
    "		"
    "		/* IE hack */"
    "		* html #left {"
    "			left: 150px;"
    "		}"
    ""
    "		/* Make the columns the same height as each other */"
    "		#container {"
    "			overflow: hidden;"
    "		}"
    ""
    "		#container .column {"
    "			padding-bottom: 1001em;"
    "			margin-bottom: -1000em;"
    "		}"
    ""
    "		/* Fix for the footer */"
    "		* html body {"
    "			overflow: hidden;"
    "		}"
    "		"
    "		* html #footer-wrapper {"
    "			float: left;"
    "			position: relative;"
    "			width: 100%;"
    "			padding-bottom: 10010px;"
    "			margin-bottom: -10000px;"
    "			background: #fff;"
    "		}"
    ""
    "		/* Aesthetics */"
    "		body {"
    "			margin: 0;"
    "			padding: 0;"
    "			font-family:Sans-serif;"
    "			line-height: 1.5em;"
    "		}"
    "		"
    "		p {"
    "			color: #555;"
    "		}"
    ""
    "		nav ul {"
    "			list-style-type: none;"
    "			margin: 0;"
    "			padding: 0;"
    "		}"
    "		"
    "		nav ul a {"
    "			color: darkgreen;"
    "			text-decoration: none;"
    "		}"
    ""
    "		#header, #footer {"
    "			font-size: large;"
    "			padding: 0.3em;"
    "			background: #BCCE98;"
    "		}"
    ""
    "		#left {"
    "			background: #DAE9BC;"
    "		}"
    "		"
    "		#right {"
    "			background: #F7FDEB;"
    "		}"
    ""
    "		#center {"
    "			background: #fff;"
    "		}"
    ""
    "		#container .column {"
    "			padding-top: 1em;"
    "		}"
    "		"
    "	</style>"
    " "
    "    <script type='text/javascript'>"
    "		/* ============================="
    "		This script generates sample text for the body content. "
    "		You can remove this script and any reference to it. "
    "		 ============================= */"
    "		var bodyText=['The smaller your reality, the more convinced you are that you know everything.', 'If the facts don't fit the theory, change the facts.', 'The past has no power over the present moment.', 'This, too, will pass.', '</p><p>You will not be punished for your anger, you will be punished by your anger.', 'Peace comes from within. Do not seek it without.', '<h3>Heading</h3><p>The most important moment of your life is now. The most important person in your life is the one you are with now, and the most important activity in your life is the one you are involved with now.']"
    "		function generateText(sentenceCount){"
    "			for (var i=0; i<sentenceCount; i++)"
    "			document.write(bodyText[Math.floor(Math.random()*7)]+' ')"
    "		}"
    "	</script>"
    "</head>"
" "
"<body>"
" "
"<header id='header'><p>Header...</p></header>"
" "
"<div id='container'>"
    " "
    "    <main id='center' class='column'>"
        "        <article>"
            " "
            "            <h1>Heading</h1>"
            "            <p><script>generateText(50)</script></p>"
            " "
            "        </article>"
        "    </main>"
    " "
    "    <nav id='left' class='column'>"
        "        <h3>Left heading</h3>"
        "        <ul>"
            "            <li><a href='#'>Link 1</a></li>"
            "            <li><a href='#'>Link 2</a></li>"
            "            <li><a href='#'>Link 3</a></li>"
            "            <li><a href='#'>Link 4</a></li>"
            "            <li><a href='#'>Link 5</a></li>"
            "        </ul>"
        "        <h3>Left heading</h3>"
        "        <ul>"
            "            <li><a href='#'>Link 1</a></li>"
            "            <li><a href='#'>Link 2</a></li>"
            "            <li><a href='#'>Link 3</a></li>"
            "            <li><a href='#'>Link 4</a></li>"
            "            <li><a href='#'>Link 5</a></li>"
            "        </ul>"
        " "
        "    </nav>"
    " "
    "    <div id='right' class='column'>"
        "        <h3>Right heading</h3>"
        "        <p><script>generateText(1)</script></p>"
        "    </div>"
    " "
    "</div>"
" "
"<div id='footer-wrapper'>"
    "    <footer id='footer'><p>Footer...</p></footer>"
    "</div>"
" "
"</body>"
" "
"</html>";
}
