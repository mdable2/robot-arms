/*
 *  Simple HTTP get webclient test
 *  Tutorial from: https://learn.adafruit.com/adafruit-huzzah-esp8266-breakout/using-arduino-ide
 */
 
#include <ESP8266WiFi.h>
 
const char* ssid     = "SM-G950U945";
const char* password = "6303918673";
 
const char* host = "192.168.43.235";
const int httpPort = 65432;

// Use WiFiClient class to create TCP connections
WiFiClient client;
 
void setup() {
  Serial.begin(115200);
  delay(100);
 
  // We start by connecting to a WiFi network
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  Serial.print("connecting to ");
  Serial.println(host);
}
 
int value = 0;
 
void loop() {

  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
}
