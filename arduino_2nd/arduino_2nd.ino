#include <Servo.h>    // servo motor header

Servo secondServo;

int motorPin = 10;     // motor pin

int pos = 90;          // servo motor at the entrance of machine

const int BAUD_RATE = 9600; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
  secondServo.attach(motorPin);     // attach motor pin    
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()) {               // read parameter from Matlab
    char ch = Serial.read();             // assign to variable ch 
    
    // if the garbage is estimated as glass
    if(ch == 2) {
      for(pos = 90; pos < 180; pos++) {        
        secondServo.write(pos);           // move 1st servo motor to left
        delay(10);
      }          
                               
    delay(1000);                          // wait the garbage falling
    pos = 90;                             // set the servo motor to initial pos
    secondServo.write(pos);
    }
    
    // if the garbage is estimated as plastic
    else if(ch == 1) {
      for(pos=90; pos > 0; pos--) {
        secondServo.write(pos);           // move 1st servo motor to left
        delay(10);
      }             
      
      delay(1000);                        // wait the garbage falling  
      pos = 90;                           // set the servo motor to initial pos
      secondServo.write(pos);
    }
  }    
}

