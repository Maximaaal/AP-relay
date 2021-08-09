#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

#include <Adafruit_MCP23017.h>

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

Adafruit_MCP23017 mcp;


const int led = 2;
int sequenceStart = 0;



void handleRoot();              // function prototypes for HTTP handlers
void handleLED();
void killSwitch();
void handleNotFound();
void sequence();
void valve0();
void valve1();
void valve2();
void valve3();
void valve4();
void valve5();
void valve6();
void valve7();
void valve8();
void valve9();
void valve10();
void valve11();

void setup(void) {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  mcp.begin();

  for (int ledPin = 0; ledPin < 11; ledPin++) {
    mcp.pinMode(ledPin, OUTPUT);

  }
  
  wifiMulti.addAP("TP-Link_5474", "96542194");
  wifiMulti.addAP("TP-LINK_AB61A6", "p6hfkp5401");   // add Wi-Fi networks you want to connect to
  wifiMulti.addAP("TELE2-E8DEF7", "CCF0140204");

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer

  if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/", HTTP_GET, handleRoot);     // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/LED", HTTP_POST, handleLED);  // Call the 'handleLED' function when a POST request is made to URI "/LED"
  server.on("/KS", HTTP_POST, killSwitch);
  server.on("/V1", HTTP_POST, valve0);
  server.on("/V2", HTTP_POST, valve1);
  server.on("/V3", HTTP_POST, valve2);
  server.on("/V4", HTTP_POST, valve3);
  server.on("/V5", HTTP_POST, valve4);
  server.on("/V6", HTTP_POST, valve5);
  server.on("/V7", HTTP_POST, valve6);
  server.on("/V8", HTTP_POST, valve7);
  server.on("/V9", HTTP_POST, valve8);
  server.on("/V10", HTTP_POST, valve9);
  server.on("/V11", HTTP_POST, valve10);
  server.on("/V12", HTTP_POST, valve11);

  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();                    // Listen for HTTP requests from clients
  if (sequenceStart == 1) {
    sequence();
  }

}

void handleRoot() {                         // When URI / is requested, send a web page with a button to toggle the LED
  server.send(200, "text/html",
              "<h2 style=\"font-family: helvetica\">FOM relay control</h2>"
              "<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"Start sequence\"></form>"
              "<form action=\"/KS\" method=\"POST\"><input type=\"submit\" value=\"Stop sequence\"></form>"
              "<br>"
              "<h3 style=\"font-family: helvetica\">individual control</h3>"
              "<form action=\"/V1\" method=\"POST\"><input type=\"submit\" value=\"Valve 1\"></form>"
              "<form action=\"/V2\" method=\"POST\"><input type=\"submit\" value=\"Valve 2\"></form>"
              "<form action=\"/V3\" method=\"POST\"><input type=\"submit\" value=\"Valve 3\"></form>"
              "<form action=\"/V4\" method=\"POST\"><input type=\"submit\" value=\"Valve 4\"></form>"
              "<form action=\"/V5\" method=\"POST\"><input type=\"submit\" value=\"Valve 5\"></form>"
              "<form action=\"/V6\" method=\"POST\"><input type=\"submit\" value=\"Valve 6\"></form>"
              "<form action=\"/V7\" method=\"POST\"><input type=\"submit\" value=\"Valve 7\"></form>"
              "<form action=\"/V8\" method=\"POST\"><input type=\"submit\" value=\"Valve 8\"></form>"
              "<form action=\"/V9\" method=\"POST\"><input type=\"submit\" value=\"Valve 9\"></form>"
              "<form action=\"/V10\" method=\"POST\"><input type=\"submit\" value=\"Valve 10\"></form>"
              "<form action=\"/V11\" method=\"POST\"><input type=\"submit\" value=\"Valve 11\"></form>"
              "<form action=\"/V12\" method=\"POST\"><input type=\"submit\" value=\"Valve 12\"></form>"


             );
}

void handleLED() {                          // If a POST request is made to URI /LED
  //  mcp.digitalWrite(0, !mcp.digitalRead(0));     // Change the state of the LED

  Serial.println("Sequence started");
  delay(500);
  sequenceStart = 1;

  server.sendHeader("Location", "/");
  server.send(303);


}


const uint16_t interval[] {2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000};
static byte state = 7;

void sequence() {

  static uint32_t previousMillis = 0;



  if (millis() - previousMillis >= interval[state]) {

    state++;
    state = state % 8;
    Serial.print(F("state=")); 
    Serial.println(state);

    
    switch (state)
    {
      case 0:
        mcp.digitalWrite(random(0, 11), HIGH);
        break;
      case 1:
        mcp.digitalWrite(random(0, 11), HIGH);
        break;
      case 2:
        mcp.digitalWrite(random(0, 11), HIGH);
        break;
      case 3:
        mcp.digitalWrite(random(0, 11), HIGH);
        break;
      case 4:
        mcp.digitalWrite(random(0, 11), HIGH);
        break;
      case 5:
        mcp.digitalWrite(random(0, 11), HIGH);
        break;
      case 6:
        mcp.digitalWrite(random(0, 11), HIGH);
        break;
      case 7:
        mcp.digitalWrite(random(0, 11), HIGH);
        break;
    }
    previousMillis = millis();
  }
  if (state == 7) {
    delay(2000);
    exit(0);
  }
}

void killSwitch() {

  Serial.println("Sequence stopped");

  for (int i = 0; i < 11; i++) {
    mcp.digitalWrite(i, LOW);
  }

  server.sendHeader("Location", "/");
  server.send(303);

  delay(500);
  wdt_disable();
  wdt_enable(WDTO_15MS);
  while (1) {}
}

void valve0() {
  mcp.digitalWrite(0, !mcp.digitalRead(0));
  server.sendHeader("Location", "/");
  server.send(303);
}

void valve1() {
  mcp.digitalWrite(1, !mcp.digitalRead(1));
  server.sendHeader("Location", "/");
  server.send(303);
}

void valve2() {
  mcp.digitalWrite(2, !mcp.digitalRead(2));
  server.sendHeader("Location", "/");
  server.send(303);
}

void valve3() {
  mcp.digitalWrite(3, !mcp.digitalRead(3));
  server.sendHeader("Location", "/");
  server.send(303);
}

void valve4() {
  mcp.digitalWrite(4, !mcp.digitalRead(4));
  server.sendHeader("Location", "/");
  server.send(303);
}

void valve5() {
  mcp.digitalWrite(5, !mcp.digitalRead(5));
  server.sendHeader("Location", "/");
  server.send(303);
}

void valve6() {
  mcp.digitalWrite(6, !mcp.digitalRead(6));
  server.sendHeader("Location", "/");
  server.send(303);
}

void valve7() {
  mcp.digitalWrite(7, !mcp.digitalRead(7));
  server.sendHeader("Location", "/");
  server.send(303);
}

void valve8() {
  mcp.digitalWrite(8, !mcp.digitalRead(8));
  server.sendHeader("Location", "/");
  server.send(303);
}

void valve9() {
  mcp.digitalWrite(9, !mcp.digitalRead(9));
  server.sendHeader("Location", "/");
  server.send(303);
}

void valve10() {
  mcp.digitalWrite(10, !mcp.digitalRead(10));
  server.sendHeader("Location", "/");
  server.send(303);
}

void valve11() {
  mcp.digitalWrite(11, !mcp.digitalRead(11));
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
