
/* Tutorial from Keyestudio
 * https://wiki.keyestudio.com/KS0320_Keyestudio_Electromagnet_Module_(Black_and_Environmental-friendly)#Example_Code
 */
 
int Solenoid = 3;    //define digital port 3

void setup()  
{        
  pinMode(Solenoid, OUTPUT);  //set Solenoid as OUTPUT
}

void loop()                     
{
  digitalWrite(Solenoid, HIGH);// Solenoid is effective
  delay(2000); // delay 2S
  digitalWrite(Solenoid, LOW);  // Solenoid is non-effective
  delay(2000);  //delay 2S      
}
