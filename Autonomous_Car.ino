// dcosta@britishschool.g12.br
// 4 wheel chassis Autonomous
// 24 July 2019

#define leftLeftSensor A15
#define leftSensor A14
#define middleSensor A13
#define rightSensor A12
#define rightRightSensor A11

int motor1Enable = 2; // Motor ports
int motor1Port1 = 22;
int motor1Port2 = 23;
int motor2Enable = 3;
int motor2Port1 = 25;
int motor2Port2 = 24;
int motor3Enable = 4;
int motor3Port1 = 26;
int motor3Port2 = 27;
int motor4Enable = 5;
int motor4Port1 = 29;
int motor4Port2 = 28;
char data = 0;  // Bluetooth data

void setup() {
  Serial.begin(9600);
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
}

void activateMotor(int enablePin, int motorPort1, int motorPort2, int
enable, byte motora, byte motorb, int delayTime = 0){
  //if(enable >200){
  //  enable=200;  //Protection against overvoltage
  //}
  analogWrite(enablePin,enable);
  digitalWrite(motorPort1,motora);
  digitalWrite(motorPort2,motorb);
  delay(delayTime);
}


void walkForward(){
  activateMotor(motor1Enable,motor1Port1,motor1Port2,250,HIGH,LOW);
  activateMotor(motor2Enable,motor2Port1,motor2Port2,250,HIGH,LOW);
  activateMotor(motor3Enable,motor3Port1,motor3Port2,18,HIGH,LOW);
  activateMotor(motor4Enable,motor4Port1,motor4Port2,15,HIGH,LOW);
}

void walkBackwards(){
  activateMotor(motor1Enable,motor1Port1,motor1Port2,250,LOW,HIGH);
  activateMotor(motor2Enable,motor2Port1,motor2Port2,250,LOW,HIGH);
  activateMotor(motor3Enable,motor3Port1,motor3Port2,18,LOW,HIGH);
  activateMotor(motor4Enable,motor4Port1,motor4Port2,15,LOW,HIGH);
}

void turnLeft(){
  activateMotor(motor1Enable,motor1Port1,motor1Port2,250,HIGH,LOW);
  activateMotor(motor2Enable,motor2Port1,motor2Port2,250,LOW,HIGH);
  activateMotor(motor3Enable,motor3Port1,motor3Port2,18,LOW,HIGH);
  activateMotor(motor4Enable,motor4Port1,motor4Port2,15,HIGH,LOW);
}

void turnRight(){
  activateMotor(motor1Enable,motor1Port1,motor1Port2,250,LOW,HIGH);
  activateMotor(motor2Enable,motor2Port1,motor2Port2,250,HIGH,LOW);
  activateMotor(motor3Enable,motor3Port1,motor3Port2,18,HIGH,LOW);
  activateMotor(motor4Enable,motor4Port1,motor4Port2,15,LOW,HIGH);
}

void stopCar(){
  activateMotor(motor1Enable,motor1Port1,motor1Port2,0,LOW,LOW);
  activateMotor(motor2Enable,motor2Port1,motor2Port2,0,LOW,LOW);
  activateMotor(motor3Enable,motor3Port1,motor3Port2,0,LOW,LOW);
  activateMotor(motor4Enable,motor4Port1,motor4Port2,0,LOW,LOW);
}

void bluetoothControl(){
    if(Serial.available() > 0){
    data = Serial.read();
    if(data == '2'){  //Forward
      walkForward();
    }
    else if(data == '1'){ //Left
      turnLeft();
    }
    else if(data == '3'){ //Right
      turnRight();
    }
    else if(data == '4'){ //Backwards
      walkBackwards();
    }
    else{
      stopCar();
    }
  }
}

void loop() {
  Serial.print("Readings: ");
  Serial.print(analogRead(leftLeftSensor));
  Serial.print(" ");
  Serial.print(analogRead(leftSensor));
  Serial.print(" ");  
  Serial.print(analogRead(middleSensor));
  Serial.print(" ");  
  Serial.print(analogRead(rightSensor));
  Serial.print(" ");  
  Serial.println(analogRead(rightRightSensor));
}
