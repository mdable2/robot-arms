#include <Servo.h>
#include <LiquidCrystal.h>

Servo base, ud, fb;
int pos;
int start = 1;

const int magPin = 12;

const int statePin = 2; 

/* FSM:
   0 Awaiting Startup - 204, 204, 0
   0 -> -1- Start up (Connecting to wifi) - 204, 102, 0
   1 -> -2- Server built - 102, 204, 0
   2 -> -3- Game server connected - 0, 204, 0
   3 -> -4- Game in progress - 0, 102, 204
   4 -> -2- "Computer restart" 
*/
int prevState = 0;
int state = 0;

/* Values for RGB LED status indicator */
const int rP = 3;
const int gP = 4;
const int bP = 5;

const int rs = 5, en = 6, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  
  /* Robot Arm Servo & electro magnet Setup */
  base.attach(11); //Orange wire
  fb.attach(10); //White wire
  ud.attach(9); //Yellow wire
  pinMode(magPin, OUTPUT);

  /* State change setup */
  pinMode(statePin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(statePin), stateReset, CHANGE);

  /* Status LED setup */
  pinMode(rP, OUTPUT);
  pinMode(gP, OUTPUT);
  pinMode(bP, OUTPUT);
  updateStatusLED();
  
  /* Setup serial & send network info to ESP8266 */
  Serial.println("StupidInternet");
  delay(100);
  Serial.println("Catpeople123?");
  char buf[32];
  Serial.readBytes(buf, 32);
  Serial.println(buf);
}

void loop() {
  switch (state) {
    case 0:
      break;
    case 1:
      //updateLCD();
      connectToWifi();
      break;
    case 2:
      //updateLCD();
      break;
    case 3:
    case 4:
      updateStatusLED();
      break;
    default:  break;
  }
}


void moveArm(char select, int pos) {
  int currPos;
  if (select != 'm') pos = map(pos, 0, 180, 1000, 2000);
  switch (select) {
         case 'b':
          currPos = map(base.read(), 0, 180, 1000, 2000);
          if (currPos < pos)
           for(int i = currPos+1; i <= pos; i++) {base.writeMicroseconds(i); delay(1);}
          else if (currPos > pos)
            for(int i = currPos-1; i >= pos; i--) {base.writeMicroseconds(i); delay(1);}
          else
            Serial.println("BASE: Already at pos.");
            
          //Serial.println("BASE: Moved from " + currPos + " to " + base.read());
          break;
          
         case 'f':
          currPos = map(fb.read(), 0, 180, 1000, 2000);
          if (currPos < pos)
           for(int i = currPos+1; i <= pos; i++) {fb.writeMicroseconds(i); delay(1);}
          else if (currPos > pos)
            for(int i = currPos-1; i >= pos; i--) {fb.writeMicroseconds(i); delay(1);}
          else
            Serial.println("FB: Already at pos.");
            
          //Serial.println("FB: Moved from " + currPos + " to " + fb.read());
          break;
          
         case 'u':
          currPos = map(ud.read(), 0, 180, 1000, 2000);
          if (currPos < pos)
           for(int i = currPos+1; i <= pos; i++) {ud.writeMicroseconds(i); delay(1);}
          else if (currPos > pos)
            for(int i = currPos-1; i >= pos; i--) {ud.writeMicroseconds(i); delay(2);}
          else
            Serial.println("UD: Already at pos.");
            
          //Serial.println("UD: Moved from " + currPos + " to " + ud.read());
          break;
          
        case 'm':
          if (pos == 1)
             digitalWrite(magPin, HIGH);
          else
            digitalWrite(magPin, LOW);
          break;
          
        default: break;
      }       
} 

void connectToWifi() {
  
}

void stateReset() {
  prevState = state;
  
  if (state = 0) state = 1;
  else state = 2;  
  
  updateStatusLED();
}

void updateStatusLED() {
  switch (state) {
    case 0:
      analogWrite(rP, 255);
      analogWrite(gP, 0);
      analogWrite(bP, 0);
      break;
    case 1:
      analogWrite(rP, 255);
      analogWrite(gP, 255);
      analogWrite(bP, 0);
      break;
    case 2:
      analogWrite(rP, 0);
      analogWrite(gP, 0);
      analogWrite(bP, 255);
      break;
    case 3:
      analogWrite(rP, 0);
      analogWrite(gP, 255);
      analogWrite(bP, 0);
      break;
    case 4:
      analogWrite(rP, 0);
      analogWrite(gP, 0);
      analogWrite(bP, 255);
      break;
    default:
      analogWrite(rP, 255);
      analogWrite(gP, 0);
      analogWrite(bP, 0);
      break;  
  }
}
