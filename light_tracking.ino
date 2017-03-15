/*
  Testing RC Connection with 5 channels
  2/22/17 by Brian Patton
  Captures the Pulse signal from a RC controller
  Requires a tri color LED connected to pins 23-21
  Feel free to do whatever you want with this code example
*/
#include <Servo.h>

// Create Variables to hold the Receiver signals
int Ch1, Ch2, Ch3, Ch4, Ch5, Ch6;
int CalcHold;        //Variable to temp hold calculations for steering stick corections
int Rwheel; //1400-1700, 1550 is middle
int Lwheel; //1200-1800, 1500 is middle
int ambi = 600; //value for "ambient" light
int stopdist = 240; //value for stopping distance ~1 ft 
const int LED1 = 23;
const int LED2 = 22;
const int LED3 = 21;

// Create Servo Objects as defined in the Servo.h files
Servo L_Servo;  // Servo DC Motor Driver (Designed for RC cars)
Servo R_Servo;  // Servo DC Motor Driver (Designed for RC cars)

//**************************************************************
//*****************  Setup  ************************************
//**************************************************************
void setup() {
  // Set the pins that the transmitter will be connected to all to input
  pinMode(12, INPUT); //I connected this to Chan1 of the Receiver
  pinMode(11, INPUT); //I connected this to Chan2 of the Receiver
  pinMode(10, INPUT); //I connected this to Chan3 of the Receiver
  pinMode(9, INPUT); //I connected this to Chan4 of the Receiver
  pinMode(8, INPUT); //I connected this to Chan5 of the Receiver
  pinMode(7, INPUT); //I connected this to Chan6 of the Receiver
  pinMode(13, OUTPUT); //Onboard LED to output for diagnostics

  pinMode(LED1, OUTPUT); //external LED to output for diagnostics
  pinMode(LED2, OUTPUT); //external LED to output for diagnostics
  pinMode(LED3, OUTPUT); //external LED to output for diagnostics

  // Attach Speed controller that acts like a servo to the board
  R_Servo.attach(0); //Pin 0
  L_Servo.attach(1); //Pin 1

  //Flash the LED on and Off 10x before entering main loop
  for (int i = 0; i < 10; i++) {
    digitalWrite(13, HIGH);
    delay(200);
    digitalWrite(13, LOW);
    delay(200);
  }
  //Flash the LED on and Off 10x End
  Serial.begin(9600);

  
  

  Rwheel = map(Rwheel, 1000, 2000, 1400, 1700); //over 1500 is reverse, 1550 should be stop
  Lwheel = map(Lwheel, 1000, 2000, 1200, 1800); //over 1500 is forward
}
//************************  loop()  ****************************
//**********************  Main Loop  ***************************
//**************************************************************
void loop()
{ 
  int L_sensor = analogRead(A0); //variable for left light sensor (get proper pin input)
  L_sensor = map(L_sensor, 0, 1000, 50, 800);
  int R_sensor = analogRead(A1); //variable for right light sensor (get proper pin input)
  R_sensor = map(R_sensor, 0, 1000, 200, 1200);
  const int P_sensor = analogRead(A2); //variable for prox sensor (get proper pin input)

//  Serial.print(L_sensor);
//  Serial.print(" ");
//  Serial.print(R_sensor);
//  Serial.print(" ");
//  Serial.println(P_sensor);
//
//  delay(100);

delay(50);

  if(P_sensor > stopdist) //stop
  {
    R_Servo.writeMicroseconds(1550);
    L_Servo.writeMicroseconds(1500);
    delay(10);
    return; 
  }  
  else if(L_sensor >= ambi && R_sensor >= ambi) //spin
  {
    R_Servo.writeMicroseconds(1623);
    L_Servo.writeMicroseconds(1556);
    delay(10);
    return;
  }
  else if(L_sensor < ambi && R_sensor - L_sensor > 150) //left
  {
    R_Servo.writeMicroseconds(1620);
    L_Servo.writeMicroseconds(1360);
    delay(10);
    return;
  }
  else if(R_sensor < ambi && L_sensor - R_sensor > 150) //right
  {
    R_Servo.writeMicroseconds(1635);
    L_Servo.writeMicroseconds(1290);
    delay(10);
    return;
  }
  else if(L_sensor < ambi && R_sensor < ambi) //straight
  {
    R_Servo.writeMicroseconds(1640);
    L_Servo.writeMicroseconds(1440);
    delay(10);
    return;
  }

}

//
void SetLimits() {
  if (Lwheel < 1000) {// Can be set to a value you don't wish to exceed
    Lwheel = 1000;    // to adjust maximums for your own robot
  }
  if (Lwheel > 2000) {// Can be set to a value you don't wish to exceed
    Lwheel = 2000;    // to adjust maximums for your own robot
  }
  if (Rwheel < 1000) {// Can be set to a value you don't wish to exceed
    Rwheel = 1000;    // to adjust maximums for your own robot
  }
  if (Rwheel > 2000) {// Can be set to a value you don't wish to exceed
    Rwheel = 2000;    // to adjust maximums for your own robot
  }
//  pulseMotors();
}
//*******************   pulseMotors  ***************************
//pulses either mapped or direct signals generated from Mixlimits
//**************************************************************
//void pulseMotors() {
  //un-comment the next two line to drive the wheels directly with the MaxLimits Set
//    R_Servo.writeMicroseconds(Rwheel);
//    L_Servo.writeMicroseconds(Lwheel);

  //un-comment the next two to map a control range.
  //*** Take the standard range of 1000 to 2000 and frame it to your own minimum and maximum
  //*** for each wheel.
//  Rwheel = map(Rwheel, 1000, 2000, 1400, 1700); //over 1500 is reverse
//  Lwheel = map(Lwheel, 1000, 2000, 1200, 1800); //over 1500 is forward
//  R_Servo.writeMicroseconds(Rwheel);
//  L_Servo.writeMicroseconds(Lwheel);

  // un-comment this line do display the value being sent to the motors
  //  PrintWheelCalcs(); //REMEMBER: printing values slows reaction times

//}
//*****************  PrintWheelCalcs()  ************************
//*******  Prints calculated wheel output values  **************
//**************************************************************
void PrintWheelCalcs() {
  Serial.print("Right Wheel = ");
  Serial.println(Rwheel);
  Serial.print("Left Wheel = ");
  Serial.println(Lwheel);
  Serial.println(" ");
}
//********************  TestWheels()  **************************
//*  Direct call to Servos to test wheel speed and direction  **
//**************************************************************
//void TestWheels() {
//  for (int i = 1000; i <= 1500; i += 10) {
//    R_Servo.writeMicroseconds(i); // 1000-2000, 1500 should be stop
//    L_Servo.writeMicroseconds(i); // 1000-2000, 1500 should be stop
//    Serial.println(" Pulse width = " + (String)i);
//    delay(200);
//  }
//
//  digitalWrite(13, HIGH);
//  for (int i = 5; i >= 0; i--) {
//    Serial.println("Holding at 1500 for  " + (String)i + " more seconds.");
//    delay(1000);
//  }
//  digitalWrite(13, LOW);
//
//  for (int i = 1500; i <= 2000; i += 10) {
//    R_Servo.writeMicroseconds(i); // 1000-2000, 1500 should be stop
//    L_Servo.writeMicroseconds(i); // 1000-2000, 1500 should be stop
//    Serial.println(" Pulse width = " + (String)i);
//    delay(200);
//  }
//  R_Servo.writeMicroseconds(1500); // 1000-2000, 1500 should be stop
//  L_Servo.writeMicroseconds(1500); // 1000-2000, 1500 should be stop
//
//  while (true) {
//    Serial.println(" Holding Pulse width at 1500 ");
//    digitalWrite(13, HIGH);
//    delay(1000);
//    digitalWrite(13, LOW);
//    delay(100);
//  }
//}

////**********************  PrintRC()  ***************************
////***  Simply print the collected RC values for diagnostics  ***
////**************************************************************
//void PrintRC()
//{ // print out the values you read in:
//  Serial.println(" RC Control Mode ");
//  Serial.print("Value Ch1 = ");
//  Serial.println(Ch1);
//  Serial.print("Value Ch2 = ");
//  Serial.println(Ch2);
//  Serial.print("Value Ch3 = ");
//  Serial.println(Ch3);
//  Serial.print("Value Ch4 = ");
//  Serial.println(Ch4);
//  Serial.print("Control = ");
//  Serial.println(Ch5);
//  Serial.print("Value Ch6 = ");
//  Serial.println(Ch6);
//  Serial.println(" ");
//  delay(1000);
//}



