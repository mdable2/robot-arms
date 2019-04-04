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
  
  /*
  delay(5000);
  ++value;
 
  while(client.available()){
    char s = client.read();
    if (s == -1) {
      Serial.print("data not receieved");
    }
    Serial.print("data received: " + s);
  }
  /*
  // We now create a URI for the request
  String url = "/testwifi/index.html";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  */
  
  //Serial.println("closing connection");
}
