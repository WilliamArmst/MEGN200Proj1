#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

// define pins
#define LEFT_SPEED 6
#define LEFT_DRIVE_1 5
#define LEFT_DRIVE_2 7
#define RIGHT_SPEED 3
#define RIGHT_DRIVE_1 2
#define RIGHT_DRIVE_2 4
#define ARM_SERVO A0
#define CLAMP_SERVO A1

struct DataPacket {
  int rightX = 0;
  int rightY = 0;
  int rightPress = 0;
  int leftX = 0;
  int leftY = 0;
  int leftPress = 0;
};

// radio variables
RF24 radio(9, 8);
DataPacket data;
const byte address[6] = "34768";

// servo variables
Servo armServo;
Servo clampServo;

int armPos = 90;
const int armMin = 63;
const int armMax = 145;
int clampPos = 90;
const int clampMin = 65;
const int clampMax = 136;

// drive variables
int leftSpeed = 0;
int leftDrive1 = LOW;
int leftDrive2 = LOW;
int rightSpeed = 0;
int rightDrive1 = LOW;
int rightDrive2 = LOW;

void setup() {
  // setup radio
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(0, address);
  radio.startListening(); // set as receiver

  Serial.begin(9600);

  // set all pins as output (DC motor pins should already be?)
  pinMode(LEFT_SPEED, OUTPUT);
  pinMode(LEFT_DRIVE_1, OUTPUT);
  pinMode(LEFT_DRIVE_2, OUTPUT);
  pinMode(RIGHT_SPEED, OUTPUT);
  pinMode(RIGHT_DRIVE_1, OUTPUT);
  pinMode(RIGHT_DRIVE_2, OUTPUT);
  pinMode(ARM_SERVO, OUTPUT);
  pinMode(CLAMP_SERVO, OUTPUT);

  armServo.attach(ARM_SERVO);
  clampServo.attach(CLAMP_SERVO);

  armServo.write(armPos);
  clampServo.write(clampPos);
}

void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(DataPacket));
    delay(20);
    Serial.print("Data Received: Right (x, y): (");
    Serial.print(data.rightX);
    Serial.print(", ");
    Serial.print(data.rightY);
    Serial.print("); Left (x, y): (");
    Serial.print(data.leftX);
    Serial.print(", ");
    Serial.print(data.leftY);
    Serial.print("); Press Left, Press Right: ");
    Serial.print(data.leftPress);
    Serial.print(", ");
    Serial.print(data.rightPress);
    Serial.println(";");
  }

  // clamp control (right joystick, left open, right close)
  if (data.rightX > 512) {
    clampPos++;
  } else if (data.rightX < 512) {
    clampPos--;
  }
  constrain(clampPos, clampMin, clampMax);
  clampServo.write(clampPos);

  // arm control (right joystick)
  if (data.rightY > 512) {
    armPos++;
  } else if (data.rightY < 512) {
    armPos--;
  }
  constrain(armPos, armMin, armMax);
  armServo.write(armPos);

  // movement (left joystick)
  // motor speeds go from 0 to 512
  if (data.leftY > 524 /* ToDo: find deadzone to rotate in place, currently +-12 (2%) */) {
    // set drives forward
    rightDrive1 = HIGH;
    rightDrive2 = LOW;
    leftDrive1 = HIGH;
    leftDrive2 = LOW;
    rightSpeed = data.leftY - 512;
    leftSpeed = data.leftY - 512;
    if (data.leftX > 512) {
      rightSpeed -= data.leftX - 512;
    } else if (data.leftX < 512) {
      leftSpeed -= 512 - data.leftX;
    }
  } else if (data.leftY < 500 /* ToDo: find deadzone to rotate in place, currently +-12 (2%) */) {
    // set drives backward
    rightDrive1 = LOW;
    rightDrive2 = HIGH;
    leftDrive1 = LOW;
    leftDrive2 = HIGH;
    rightSpeed = 512 - data.leftY;
    leftSpeed = 512 - data.leftY;
    if (data.leftX > 512) {
      rightSpeed -= data.leftX - 512;
    } else if (data.leftX < 512) {
      leftSpeed -= 512 - data.leftX;
    }
  } else {
    // rotate in place
    if (data.leftX > 512) {
      // rotate right
      rightDrive1 = LOW;
      rightDrive2 = HIGH;
      leftDrive1 = HIGH;
      leftDrive2 = LOW;
      rightSpeed = data.leftX - 512;
      leftSpeed = data.leftX - 512;
    } else if (data.leftX < 512) {
      // rotate left
      rightDrive1 = HIGH;
      rightDrive2 = LOW;
      leftDrive1 = LOW;
      leftDrive2 = HIGH;
      rightSpeed = 512 - data.leftX;
      leftSpeed = 512 - data.leftX;
    } else {
      // not moving
      rightDrive1 = LOW;
      rightDrive2 = LOW;
      leftDrive1 = LOW;
      leftDrive2 = LOW;
    }
  }
  digitalWrite(RIGHT_DRIVE_1, rightDrive1);
  digitalWrite(RIGHT_DRIVE_2, rightDrive2);
  digitalWrite(LEFT_DRIVE_1, leftDrive1);
  digitalWrite(LEFT_DRIVE_2, leftDrive2);
  analogWrite(RIGHT_SPEED, rightSpeed);
  analogWrite(LEFT_SPEED, leftSpeed);

  delay(20);
}
