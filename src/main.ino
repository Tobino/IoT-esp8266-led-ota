#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "Arduino.h"
#include <FS.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "../config/conf";

MDNSResponder mdns;

ESP8266WebServer server(80);

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup() {

  // Serial.begin(115200);
  // Serial.println("Booting");

  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  delay(500);
  digitalWrite(4, HIGH);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    // Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    // Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    // Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    // Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    // Serial.printf("Error[%u]: ", error);
    // if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    // else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    // else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    // else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    // else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  // Serial.println("Ready");
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());

  if (mdns.begin("esp8266", WiFi.localIP())) {
    // Serial.println("MDNS responder started");
  }

  if (SPIFFS.begin()) {
    server.serveStatic("/", SPIFFS, "/index.html");
  }

  server.on("/ledON", [](){
    File file = SPIFFS.open("/ledON.html", "r");
    server.streamFile(file, "text/html");
    file.close();
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(4, LOW);
  });
  server.on("/ledOFF", [](){
    File file = SPIFFS.open("/ledOFF.html", "r");
    server.streamFile(file, "text/html");
    file.close();
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(4, HIGH);
  });

  server.onNotFound(handleNotFound);

  server.begin();
  // Serial.println("HTTP server started");


}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();
}
