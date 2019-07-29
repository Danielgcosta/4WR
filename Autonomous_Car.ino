// dcosta@britishschool.g12.br
// 4 wheel chassis Autonomous
// 24 July 2019

#include <LiquidCrystal.h>
#include <Ultrasonic.h>

// LCD config
#define rs 6
#define en 7
#define d4 30
#define d5 31
#define d6 32
#define d7 33
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Ultrasonic Sensor
#define trigger 10
#define echo 11
Ultrasonic ultrasonic(trigger, echo);
int distance = 0;

// Motor configuration
// Power thresholds 
int mot1MaxPower = 110;  
int mot2MaxPower = 140;   
int mot3MaxPower = 17;   
int mot4MaxPower = 30; 
/*int mot1MaxPower = 110;  
int mot2MaxPower = 130;   
int mot3MaxPower = 13;   
int mot4MaxPower = 30;  */
int motor1Enable = 2; // Motor ports
int motor1Port1 = 23;
int motor1Port2 = 22;
int motor2Enable = 3;
int motor2Port1 = 25;
int motor2Port2 = 24;
int motor3Enable = 4;
int motor3Port1 = 26;
int motor3Port2 = 27;
int motor4Enable = 5;
int motor4Port1 = 29;
int motor4Port2 = 28;

// Bluetooth configuration
char data = 0;  // Received data

// Radio control configuration
#define channel1 A0
#define channel2 A2
int stickThreshold = 20;
int motorPower;
int leftActivation;
int rightActivation;
  
// Infrared Sensors configuration
#define leftLeftSensor A15
#define leftSensor A14
#define middleSensor A13
#define rightSensor A12
#define rightRightSensor A11

int leftLeftLight = 0;
int leftLight = 0;
int middleLight = 0;
int rightRightLight = 0;
int rightLight = 0;
int lightThreshold = 500;

// Buzzer configuration
int buzzer = 13;
const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;

void setup() {
  Serial.begin(9600);  
  
  pinMode(channel1,INPUT);  // Radio control inputs
  pinMode(channel2,INPUT);
  
  lcd.begin(16, 2); // Columns and rows
  //lcd.noDisplay();  // Turn the display off
  
  pinMode(buzzer, OUTPUT);
  
  pinMode(motor1Enable,OUTPUT);
  pinMode(motor1Port1,OUTPUT);
  pinMode(motor1Port2,OUTPUT);
  pinMode(motor2Enable,OUTPUT);
  pinMode(motor2Port1,OUTPUT);
  pinMode(motor2Port2,OUTPUT);
  pinMode(motor3Enable,OUTPUT);
  pinMode(motor3Port1,OUTPUT);
  pinMode(motor3Port2,OUTPUT);
  pinMode(motor4Enable,OUTPUT);
  pinMode(motor4Port1,OUTPUT);
  pinMode(motor4Port2,OUTPUT);

  pinMode(leftLeftSensor,INPUT);
  pinMode(leftSensor,INPUT);
  pinMode(middleSensor,INPUT);
  pinMode(rightSensor,INPUT);
  pinMode(rightRightSensor,INPUT);

  // Initialization tone
  tone(buzzer,440,300);
  delay(300);
  tone(buzzer,880,200);
}

// **********************
// *** MOTOR ROUTINES ***

void activateMotor(int enablePin, int motorPort1, int motorPort2, int
enable, byte motora, byte motorb, int delayTime = 0){
  if(enable >200){
    enable=200;  //Protection against overvoltage
  }
  analogWrite(enablePin,enable);
  digitalWrite(motorPort1,motora);
  digitalWrite(motorPort2,motorb);
  delay(delayTime);
}

void walkForward(){
  activateMotor(motor1Enable,motor1Port1,motor1Port2,mot1MaxPower,HIGH,LOW);
  activateMotor(motor2Enable,motor2Port1,motor2Port2,mot2MaxPower,HIGH,LOW);
  activateMotor(motor3Enable,motor3Port1,motor3Port2,mot3MaxPower,HIGH,LOW);
  activateMotor(motor4Enable,motor4Port1,motor4Port2,mot4MaxPower,HIGH,LOW);
}

void walkBackwards(){
  activateMotor(motor1Enable,motor1Port1,motor1Port2,mot1MaxPower,LOW,HIGH);
  activateMotor(motor2Enable,motor2Port1,motor2Port2,mot2MaxPower,LOW,HIGH);
  activateMotor(motor3Enable,motor3Port1,motor3Port2,mot3MaxPower,LOW,HIGH);
  activateMotor(motor4Enable,motor4Port1,motor4Port2,mot4MaxPower,LOW,HIGH);
}

void turnLeft(){
  activateMotor(motor1Enable,motor1Port1,motor1Port2,mot1MaxPower,LOW,HIGH);
  activateMotor(motor2Enable,motor2Port1,motor2Port2,mot2MaxPower,HIGH,LOW);
  activateMotor(motor3Enable,motor3Port1,motor3Port2,mot3MaxPower,LOW,HIGH);
  activateMotor(motor4Enable,motor4Port1,motor4Port2,mot4MaxPower,HIGH,LOW);
}

void turnRight(){
  activateMotor(motor1Enable,motor1Port1,motor1Port2,mot1MaxPower,HIGH,LOW);
  activateMotor(motor2Enable,motor2Port1,motor2Port2,mot2MaxPower,LOW,HIGH);
  activateMotor(motor3Enable,motor3Port1,motor3Port2,mot3MaxPower,HIGH,LOW);
  activateMotor(motor4Enable,motor4Port1,motor4Port2,mot4MaxPower,LOW,HIGH);
}

void stopCar(){
  activateMotor(motor1Enable,motor1Port1,motor1Port2,0,LOW,LOW);
  activateMotor(motor2Enable,motor2Port1,motor2Port2,0,LOW,LOW);
  activateMotor(motor3Enable,motor3Port1,motor3Port2,0,LOW,LOW);
  activateMotor(motor4Enable,motor4Port1,motor4Port2,0,LOW,LOW);
}

void radioControlActivation(){
  int left = leftActivation*motorPower/100;
  int right = rightActivation*motorPower/100;
  Serial.print("M:");
  Serial.print(motorPower);
  Serial.print(" L:");  
  Serial.print(leftActivation);
  Serial.print(" R:");  
  Serial.print(rightActivation);
  Serial.print(" 1:");  
  Serial.print(left*mot1MaxPower/100);
  Serial.print(" 2:");  
  Serial.print(right*mot2MaxPower/100);
  Serial.print(" 3:");  
  Serial.print(left*mot3MaxPower/100);
  Serial.print(" 4:");  
  Serial.print(right*mot4MaxPower/100);
  Serial.print(" ");  
  Serial.print(left*mot1MaxPower/100);
  Serial.println(" ");  
  
  if(left>=0){
    activateMotor(motor1Enable,motor1Port1,motor1Port2,left*mot1MaxPower/100,HIGH,LOW); // front left
    activateMotor(motor3Enable,motor3Port1,motor3Port2,left*mot3MaxPower/100,HIGH,LOW); // rear left
  }
  else{    
    activateMotor(motor1Enable,motor1Port1,motor1Port2,abs(left)*mot1MaxPower/100,LOW,HIGH);
    activateMotor(motor3Enable,motor3Port1,motor3Port2,abs(left)*mot3MaxPower/100,LOW,HIGH);
  }
  if(right>=0){
    activateMotor(motor2Enable,motor2Port1,motor2Port2,right*mot2MaxPower/100,HIGH,LOW);  //front right
    activateMotor(motor4Enable,motor4Port1,motor4Port2,right*mot4MaxPower/100,HIGH,LOW);
  }
  else{
    activateMotor(motor2Enable,motor2Port1,motor2Port2,abs(right)*mot2MaxPower/100,LOW,HIGH);  
    activateMotor(motor4Enable,motor4Port1,motor4Port2,abs(right)*mot4MaxPower/100,LOW,HIGH);
  }
}

// ******************************
// *** RADIO CONTROL ROUTINES ***

void radioControl(){  
  lcd.setCursor(0,0);
  lcd.print("radio control   ");
  lcd.setCursor(0,1);
  lcd.print("mode            "); 
  
  long pulse1 = pulseIn(channel1,HIGH);
  long pulse2 = pulseIn(channel2,HIGH);

  // Channel 1 right stick
  // < 1450 to 1230 left
  // > 1500 to 1730 right

  if(pulse1<1430){
    leftActivation = map(pulse1,1280,1430,0,100);
    if(leftActivation > 100){
      leftActivation= 100;
    }
    else if(pulse1 < 1280){
      leftActivation = map(pulse1,1230,1280,-100,0);
      if(leftActivation < -100){        
        leftActivation = -100;
      }
    }
    rightActivation = 100;
  }
  else if(pulse1>1500){
    rightActivation = map(pulse1,1500,1650,100,0); 
    if(rightActivation > 100){
      rightActivation = 100; 
    }
    else if(pulse1 >1650){
      rightActivation = map(pulse1,1650,1750,0,-100);
      if(rightActivation < -100){
        rightActivation = -100;
      }
    } 
    leftActivation = 100;
  }
  /*
  Serial.print("1: ");
  Serial.print(pulse1);
  Serial.print(" - 2: ");
  Serial.print(pulse2);*/

  // Channel 2 acceleration
  // 2150 to 850

  stickThreshold = 20;
  motorPower = map(pulse2,2150,850,-100,100);
  if((motorPower < stickThreshold)&&(motorPower >-stickThreshold)){
    motorPower = 0;
  }
  else if(motorPower>=stickThreshold){
    motorPower = (motorPower-stickThreshold)*100/(100-stickThreshold);
  }
  else{    
    motorPower = (motorPower+stickThreshold)*100/(100-stickThreshold);
  }/*
  Serial.print(" motor power = ");
  Serial.print(motorPower);
  Serial.print(" left motor: ");
  Serial.print(leftActivation*motorPower/100);
  Serial.print(" right motor: ");
  Serial.println(rightActivation*motorPower/100);*/
  radioControlActivation();
}


// *************************
// *** BLUETOOTH ROUTINES ***

void bluetoothControl(){
  lcd.setCursor(0,0);
  lcd.print("bluetooth mode  ");
  lcd.setCursor(0,1);
  lcd.print("                ");
  
    if(Serial.available() > 0){
    data = Serial.read();
    if(data == '2'){  //Forward
      walkForward();
      walkForwardtlcdMessage();
    }
    else if(data == '1'){ //Left
      turnLeft();
      turnLeftlcdMessage();
    }
    else if(data == '3'){ //Right
      turnRight();
      turnRightlcdMessage();
    }
    else if(data == '4'){ //Backwards
      walkBackwards();
      walkBackwardslcdMessage();
    }
    else if(data == '5'){ //Infrared test (square)
        testInfrared();        
        testInfraredlcdMessage();
        tone(buzzer,880,100);
        delay(200);
        tone(buzzer,880,100);
        delay(200);
        tone(buzzer,880,100);
        delay(200);
        tone(buzzer,880,100);
        delay(200);
        tone(buzzer,880,100);
        delay(1000);
    }
    else if(data == '6'){ //line follow (triangle)
        autonomousMode();
        autonomousModelcdMessage();
        tone(buzzer,880,100);
        delay(200);
        tone(buzzer,880,100);
    }
    else if(data == '7'){
      stopCar();
    }
    else if(data == '8'){
      stopCar();
    }
    else if(data == '9'){
      stopCar();
    }
    else if(data == 'A'){
      stopCar();
    }
    else{
      stopCar();
    }
  }
}


// *********************************
// *** INFRARED SENSORS ROUTINES ***

void testInfrared(){
  Serial.print("Readings: ");
  Serial.print(analogRead(leftLeftSensor));
  Serial.print(" ");
  Serial.print(analogRead(leftSensor));
  Serial.print(" ");  
  Serial.print(analogRead(middleSensor));
  Serial.print(" ");  
  Serial.print(analogRead(rightSensor));
  Serial.print(" ");  
  Serial.print(analogRead(rightRightSensor));
}

void followLine(){
  leftLeftLight = analogRead(leftLeftSensor);  
  leftLight = analogRead(leftSensor);  
  middleLight = analogRead(middleSensor);  
  rightLight = analogRead(rightSensor);
  rightRightLight = analogRead(rightRightSensor);

  if(middleLight < lightThreshold){
    walkForward();
    walkForwardtlcdMessage();
    delay(10);
  }
  else if((leftLeftLight > lightThreshold)||(leftLight > lightThreshold)){
    turnLeft();
    turnLeftlcdMessage();
    delay(10);
  }
  else if((rightRightLight > lightThreshold)||(rightLight > lightThreshold)){
    turnRight();
    turnRightlcdMessage();
    delay(10);
  }
  else if(middleLight > lightThreshold){
    tone(buzzer,880,200);
    int randomNumber = random(1,4);
    Serial.print("Random number sorted: ");
    Serial.print(randomNumber);
    if( randomNumber > 3){
      turnLeft();
      turnLeftlcdMessage();
      delay(5);
    }
    else if(randomNumber > 2){
      turnRight();
      turnRightlcdMessage();
      delay(5);
    }
    else if(randomNumber >1){
      walkBackwards();
      walkBackwardslcdMessage();
      delay(3);
    }
    else{
      walkForwardtlcdMessage();
      walkForward();
      delay(5);
    }
  }
  Serial.print(" Readings: ");
  Serial.print(leftLeftLight);
  Serial.print(" ");
  Serial.print(leftLight);
  Serial.print(" ");  
  Serial.print(middleLight);
  Serial.print(" ");  
  Serial.print(rightLight);
  Serial.print(" ");  
  Serial.println(rightRightLight);
}

// ***********************
// *** BUZZER ROUTINES ***

void beep(int note, int duration){
  tone(buzzer, note, duration);  
  //Stop tone on buzzer
  noTone(buzzer);  
  delay(50);  
}

void firstSection(){
  beep(a, 500);
  beep(a, 500); 
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150); 
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
  
  delay(500);
  
  beep(eH, 500);
  beep(eH, 500);
  beep(eH, 500); 
  beep(fH, 350);
  beep(cH, 150);
  beep(gS, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
  
  delay(500);
}

void secondSection(){
  beep(aH, 500);
  beep(a, 300);
  beep(a, 150);
  beep(aH, 500);
  beep(gSH, 325);
  beep(gH, 175);
  beep(fSH, 125);
  beep(fH, 125); 
  beep(fSH, 250);
  
  delay(325);
  
  beep(aS, 250);
  beep(dSH, 500);
  beep(dH, 325); 
  beep(cSH, 175); 
  beep(cH, 125); 
  beep(b, 125); 
  beep(cH, 250); 
  
  delay(350);
}

void sing(){
  //Play first section
  firstSection();
  
  //Play second section
  secondSection();
  
  //Variant 1
  beep(f, 250); 
  beep(gS, 500); 
  beep(f, 350); 
  beep(a, 125);
  beep(cH, 500);
  beep(a, 375); 
  beep(cH, 125);
  beep(eH, 650);
  
  delay(500);
  
  //Repeat second section
  secondSection();
  
  //Variant 2
  beep(f, 250); 
  beep(gS, 500); 
  beep(f, 375); 
  beep(cH, 125);
  beep(a, 500); 
  beep(f, 375); 
  beep(cH, 125);
  beep(a, 650); 
  
  delay(650);
}

void turnLeftlcdMessage(){
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print("turn left");
  lcd.setCursor(0,1);
  lcd.print("                ");
}

void turnRightlcdMessage(){
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print("turn right");
  lcd.setCursor(0,1);
  lcd.print("                ");
}

void walkForwardtlcdMessage(){
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print("turn left");
  lcd.setCursor(0,1);
  lcd.print("                ");
}

void walkBackwardslcdMessage(){
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print("turn left");
  lcd.setCursor(0,1);
  lcd.print("                ");
}

void autonomousModelcdMessage(){
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print("autonomous mode");
  lcd.setCursor(0,1);
  lcd.print("                ");
}

void testInfraredlcdMessage(){
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print("infrared test");
  lcd.setCursor(0,1);
  lcd.print("                ");
}

void autonomousMode(){
  //mot1MaxPower = mot1MaxPower/2; // Power adjustments  
  //mot2MaxPower = mot2MaxPower/2;   
  //mot3MaxPower = mot3MaxPower/2;   
  //mot4MaxPower = mot4MaxPower/2;  
  followLine();  
  // avoidObstacles();
}

void ultrasonicSensor(){
  long microsec = ultrasonic.timing();
  distance = ultrasonic.convert(microsec, Ultrasonic::CM);
  
  Serial.print("Distance = ");
  Serial.print(distance);
  Serial.print(" cm");

  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print("dist = ");
  lcd.setCursor(0,1);
  lcd.print(distance);
  lcd.print(" cm");
}

void loop() {
  // for bluetooth control, use bluetoothControl()
  // for radio control, use radioControl()
  // for autonomous mode, use autonomousMode()
  
  //bluetoothControl();
  radioControl();
  //autonomousMode();  
}
