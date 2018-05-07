#include <Servo.h>   // servo motor header

Servo firstServo;    // servo motor at the entrance of machine

int trigPin = 12;     // trigger pin of ultrasonic sensor
int echoPin = 7;      // echo pin of ultrasonic sensor
int CDS = A0;         // light sensor pin
int motorPin = 9;     // motor pin

int pos = 90;         // servo motor initial pos
int light = 0;        // ligth value
int go = 0;           // Matlab send parameter

int BAUD_RATE = 9600; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
  pinMode(trigPin,OUTPUT);       // set trig pin into OUTPUT mode
  pinMode(echoPin,INPUT);        // set echo pin into INPUT mode
  pinMode(CDS,INPUT);            // set light sensor pin into INPUT mode  
  firstServo.attach(motorPin);   // attach motor pin  
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned int duration;            // measured time from ultrasonic pin
  int distance;                     // calculated distance from duration
  
  digitalWrite(trigPin,LOW);        // to initialize ultrasonic sensor
  delayMicroseconds(2);           
  digitalWrite(trigPin,HIGH);       // generate trigger wave
  delayMicroseconds(5);
  digitalWrite(trigPin,LOW);      
  duration = pulseIn(echoPin,HIGH); // measure time while echoPin receives HIGH signal
  distance=(duration/2)/29.1;       // convert time unit into distance unit 
                                    
  delay(200);

  // separate metal 
  if(distance <= 30) {                 // when the garbage enters the entrance 
    light = analogRead(CDS);           // read light from metal detector
    
    if(light >= 100) {                 // when metal detector works
      for(pos = 90; pos > 0; pos--) {        
        firstServo.write(pos);         // move 1st servo motor to left 
        delay(10);
      }
      
      delay(1000);                     // wait the garbage falling
      pos = 90;                        // set the servo motor to initial pos
      firstServo.write(pos);                
    }
    // if metal detector does not work
    else {
      firstServo.write(pos);
      Serial.println(go);             // send paramter to Matlab
      
      for(pos = 90; pos < 180; pos++) {      
        firstServo.write(pos);         // move 1st servo motor to right
        delay(10);
      }

      delay(1000);                     // wait the garbage falling
      pos = 90;                        // set the servo motor to initial pos
      firstServo.write(pos);
    }
  }
}

