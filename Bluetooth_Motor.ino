#include <SoftwareSerial.h> 
#include <Wire.h> 
#include "SparkFun_MMA8452Q.h" 

const int PWB =10;
const int BIN1 = 9;
const int BIN2 =8;


// initialisation of serial input and ouput ports for the bluetooth module
SoftwareSerial BTSerial(3,2);  // RX, TX (must match your wiring) 

MMA8452Q accel; 

//Initialization of ports controlling the motor
const int AIN1 = 13;           //control pin 1 on the motor driver for the right motor 
const int AIN2 = 12;           //control pin 2 on the motor driver for the right motor 
const int PWMA = 11;            //speed control pin on the motor driver for the right motor 
const int BIN1 = 8;           //control pin 1 on the motor driver for the right motor 
const int BIN2 = 9;           //control pin 2 on the motor driver for the right motor 
const int PWMB = 10;            //speed control pin on the motor driver for the right motor
const int trigPin = 6;          //trigger pin for the ultrasonic distance sensor
const int echoPin = 7;          //echo pin for the ultrasonic distance sensor

//Initialisation of constant variables
int motorSpeed = 250;       //starting speed for the motor 
float duration, distance;

void setup() { 
  pinMode(AIN1, OUTPUT);    //
  pinMode(AIN2, OUTPUT);    // these three lines set the Motor pins to output status
  pinMode(PWMA, OUTPUT);    //
  pinMode(BIN1, OUTPUT);    //
  pinMode(BIN2, OUTPUT);    // these three lines set the Motor pins to output status
  pinMode(PWMB, OUTPUT);    //
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  BTSerial.begin(9600);     //this line begins serial communication with the Bluetooth serial object 
 
  Serial.begin(9600);       //this line allows the arduino to report back to the laptop(will probably be removed and is mostly useful for debugging)

  Wire.begin();

  if (accel.begin() == false) {
    Serial.println("Not Connected. Please check connections and read the hookup guide.");
    while (1);
  }

} 

void loop() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*0.0343)/2;
  Serial.print(distance);
  Serial.print("m");
  delay(100);
  if (distance<=5){
          spinMotor1(0);
          spinMotor2(0);
  }else{
    if (BTSerial.available()) {

          char command = (char)BTSerial.read(); 
          Serial.println("Received: " + String(command)); 

          switch (command){                  //My idea here is to use the case either to directly make commands, But I also thought we could use it to recover full words
                                        //by using it to differentiate between single letter commands or storing a letter to a string.
            case 'F':
          // F = full forward
            spinMotor1(255);
            spinMotor2(255);
               break;
            case 'B':
          // B = full backward
           spinMotor1(-255);
            spinMotor2(-255);
               break;
            case 'S':
          // S = Complete Stop
            spinMotor1(0);
            spinMotor2(0);
              break;
            case 'L':
          //  L = Hard Left
            spinMotor1(255);     //incomplete, need to get code spinning both motors
            spinMotor2(55);  
              break;
            case 'R':
          // R = Hard Right
            spinMotor1(55);
            spinMotor2(255);
              break;
            case 'Q':         //Extra placeholders for more functionality
              break;
            case 'W':
              break;
            case 'E':
              break;
            case 'T':
              break;
            case 'Y':
              break;
        }
    } 
  }
    
    if (accel.available()) {      // Wait for new data from accelerometer
    // Acceleration of x, y, and z directions in g units
    Serial.print(accel.getCalculatedX(), 3);
    Serial.print("\t");
    Serial.print(accel.getCalculatedY(), 3);
    Serial.print("\t");
    Serial.print(accel.getCalculatedZ(), 3);
    Serial.println();}

} 
//Homebrew functions begin here
/********************************************************************************/ 
void spinMotor1(int motorSpeed)                       //function for driving the right motor 
{ 

  if (motorSpeed > 0){                                //if the motor should drive forward (positive speed)  
    digitalWrite(AIN1, HIGH);                         //set pin 1 to high 
    digitalWrite(AIN2, LOW);                          //set pin 2 to low 
  }
  else if (motorSpeed < 0){ 
    digitalWrite(AIN1, LOW);                          //set pin 1 to low 
    digitalWrite(AIN2, HIGH);                         //set pin 2 to high 
  }else{                                              //if the motor should stop 
    digitalWrite(AIN1, LOW);                          //set pin 1 to low 
    digitalWrite(AIN2, LOW);                          //set pin 2 to low 
  }

  analogWrite(PWMA, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed 
}

void spinMotor2(int motorSpeed)                       //function for driving the right motor 
{ 

  if (motorSpeed > 0){                                //if the motor should drive forward (positive speed)  
    digitalWrite(BIN1, HIGH);                         //set pin 1 to high 
    digitalWrite(BIN2, LOW);                          //set pin 2 to low 
  }
  else if (motorSpeed < 0){ 
    digitalWrite(BIN1, LOW);                          //set pin 1 to low 
    digitalWrite(BIN2, HIGH);                         //set pin 2 to high 
  }else{                                              //if the motor should stop 
    digitalWrite(BIN1, LOW);                          //set pin 1 to low 
    digitalWrite(BIN2, LOW);                          //set pin 2 to low 
  }

  analogWrite(PWMB, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed 
}
