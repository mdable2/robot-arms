// Robot 1 WiFi Card Source Code

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <LiquidCrystal.h>

const int rs = 4, en = 5, d4 = 13, d5 = 12, d6 = 14, d7 = 16;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

WiFiUDP sock;
char inUdpPacket[256];

char rx_buf[32];

String ip, sport, serverip;
String ssid, pswd;

int state;

// 8888 for robot 1, 8899 for robot 2
const int port = 8899;
 
void setup() {
  Serial.begin(9600);
  while(!Serial) {delay(1);}
  
  lcd.display();
  lcd.begin(16, 2);

  // Beginning message
  lcd.setCursor(0,0);
  lcd.print("Robots can play");
  lcd.setCursor(0,1);
  lcd.print("checkers too!");
  lcd.setCursor(0,0);
  
  state = 0;
}
 
void loop() {
  if (state == 0) {
      if ( Serial.available() ) {
        Serial.read();
        Serial.read();
        Serial.readBytes(rx_buf, 32);
  
        interpretMsg();
      }
  }
  else if(state == 2 || state == 3) {
    int packetSize = sock.parsePacket();
    
    if (packetSize) {
        int len = sock.read(inUdpPacket, 255);

        if (len > 0) {
          if (state == 2) { Serial.write("3"); state = 3; delay(5); }
          lcd.clear();
          String s = String(packetSize);
          String sp = "Packet Size: " + s;
          String packet = String(inUdpPacket);
          lcd.setCursor(0,0);
          lcd.print(sp);
          lcd.setCursor(0,1);
          lcd.print(packet);
          lcd.setCursor(0,0);
          Serial.write(inUdpPacket);
        }
    }
  }
}

void interpretMsg() {
  char state = rx_buf[0];
  

  if (state == '1') { // Read in Wi-Fi ssid and password data
    int getSsid = 1;
    ssid = "";
    pswd = "";
    ssid = "SM-G950U945";
    pswd = "6303918673";
    
    for (int i = 0; i < 32; i++) {
      lcd.clear();
      lcd.print(String(rx_buf[i]));
      delay(500);
    }
    connectToWiFi();
  }
}

void connectToWiFi() {
  lcd.clear();
  lcd.print("Connecting");
  lcd.setCursor(0,1);
  lcd.print("to WiFi.");
  lcd.setCursor(0,0);

  delay(500);

  lcd.clear();
  lcd.print(ssid);
  lcd.setCursor(0,1);
  lcd.print("to WiFi.");
  lcd.setCursor(0,0);
  
  WiFi.begin(ssid.c_str(), pswd.c_str());
  
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  lcd.clear();
  lcd.print("Connected.");
  lcd.setCursor(0,1);
  lcd.print("Building Server...");
  lcd.setCursor(0,0);
  
  delay(1000);
  sock.begin(port);
  lcd.clear();
  lcd.print("Server Built.");
  delay(500);
  
  sport = String(port);
  ip = WiFi.localIP().toString();
  serverip = ip + sport;
  
  lcd.clear();
  lcd.print(ip);
  lcd.setCursor(0,1);
  lcd.print(sport);
  lcd.setCursor(0,0);
  
  state = 2;  
  Serial.write("2");
}
