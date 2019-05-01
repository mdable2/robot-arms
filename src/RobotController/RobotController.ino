#include <Servo.h>

Servo base, ud, fb;
int pos;
int start = 1;

const int magPin = 12;

const int statePin = 2; 

char rx_buf[32];

char ssidPswd[32] = "1;SM-G950U945;6303918673.";

/* FSM:
   0 Awaiting Startup
   0 -> -1- Start up (Connecting to wifi)
   1 -> -2- Server built
   2 -> -3- Game server connected
   3 -> -4- Game in progress
   4 -> -2- "Computer restart" 
*/
int prevState = 0;
int state;

/* Values for RGB LED status indicator */
const int rP = 3;
const int gP = 4;
const int bP = 5;

void setup() {
  Serial.begin(9600);
  while(!Serial) {delay(1);}
  
  /* Robot Arm Servo & electro magnet Setup */
  base.attach(11); //Orange wire
  fb.attach(10); //White wire
  ud.attach(9); //Yellow wire
  pinMode(magPin, OUTPUT);

  /* State change setup */
  pinMode(statePin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(statePin), stateReset, RISING);
  state = 0;

  /* Status LED setup */
  pinMode(rP, OUTPUT);
  pinMode(gP, OUTPUT);
  pinMode(bP, OUTPUT);
  updateStatusLED();
}

void loop() {
  if (state) {
    if (state == 1) {
        Serial.write(ssidPswd);
        
        while (!Serial.available()) { delay(1); }
        
          char rx_byte = Serial.read();
          if (rx_byte == '2') {
            state = 2;
            updateStatusLED();
          }
    }
    else if (state == 2) {
      if ( Serial.available() ) {
          char rx_byte = Serial.read();
          if (rx_byte == '3') {
            state = 3;
            updateStatusLED();
          }
      }
    }
    else if (state == 3) {
      if ( Serial.available() ) {
          analogWrite(rP, 100);
          analogWrite(gP, 0);
          analogWrite(bP, 100);
          Serial.readBytes(rx_buf, 32); // b,f,u, or m (magnet) as first char, then degrees if magnet then 1, end with period
          char select = rx_buf[0];
          String pos;
          for (int i = 1; i < 32; i++) {
            if (rx_buf[i] == '.') { break; }
              pos += rx_buf[i];
          }
          int pos2 = pos.toInt();
          moveArm(select, pos2);
      }
    }
    else if (state == 4) {
      
    }
    else { state = 2; } // set state to 2
  }
}

void makeMove() { // (x1,y1)(x2,y2)
  char x1 = rx_buf[1];  
  char y1 = rx_buf[3]; 
  char x2 = rx_buf[6];  
  char y2 = rx_buf[8]; 
  
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

void stateReset() {
  prevState = state;
  
  if (state == 0) state = 1;
  else state = 2;  
  
  Serial.println(state);
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
