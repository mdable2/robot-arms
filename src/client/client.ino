// Robot 1 WiFi Card Source Code

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
WiFiUDP sock;

char ssid[32];
char pswd[32];

String ip, serverIp;
// 8888 for robot 1, 8899 for robot 2
const int port = 8888;
 
void setup() {
  Serial.begin(9600);
  
  Serial.readBytes(ssid, 32);
  Serial.readBytes(pswd, 32);
  
  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED)
    delay(500);

  ip = WiFi.localIP().toString();
  serverIp = ip + ":8888"; // robot 1
  sock.begin(port);

  Serial.println("Connected");
  Serial.println(ip);

  delay(200);
}
 
void loop() {

}
