// Robot 1 WiFi Card Source Code

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <LiquidCrystal.h>

const int rs = 4, en = 5, d4 = 13, d5 = 12, d6 = 14, d7 = 16;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

WiFiUDP sock;

const char *rx_buf;
String rx_str = "";

String ip, sport, serverip;
String ssid, pswd;

int state;

// 8888 for robot 1, 8899 for robot 2
const int port = 8888;
 
void setup() {
  Serial.begin(9600);
  lcd.display();
  lcd.begin(16, 2);
  lcd.print("hello");
  state = 0;
}
 
void loop() {
  if (state == 0) {
      while (Serial.available() > 0) {
      rx_str = Serial.readStringUntil('.');
      rx_buf = rx_str.c_str();

      interpretMsg();
      }
  }
  else if(state == 2) {
    int packetSize = sock.parsePacket();
    if (packetSize) {
        lcd.clear();
        String s = String(packetSize);
        lcd.print(s);
    }
  }
}

void interpretMsg() {
  int len = strlen(rx_buf);
  char state = rx_buf[0];

  if (state == '1') {
    int getSsid = 1;
    for (int i = 2; i < len; i++) {
      if (rx_buf[i] == ';') { getSsid = 0; continue; }
      if (rx_buf[i] == '.') break;
      
      if (getSsid)
        ssid += rx_buf[i];
      else
        pswd += rx_buf[i];
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
  Serial.println("2");
}
