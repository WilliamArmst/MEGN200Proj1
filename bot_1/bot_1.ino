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

RF24 radio(9, 8);
DataPacket data;
const byte address[6] = "34768";

Servo armServo;
Servo clampServo;

int armPos = 90;
const int armMin = 70;
const int armMax = 130;
int clampPos = 90;
const int clampMin = 70;
const int clampMax = 120;

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
  }
  
  Serial.print("Data: Right (x, y): (");
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

  if (data.rightX > 512) {
    clampPos++;
  } else if (data.rightX < 512) {
    clampPos--;
  }
  constrain(clampPos, clampMin, clampMax);

  if (data.rightY > 512) {
    armPos++;
  } else if (data.rightY < 512) {
    armPos--;
  }
  constrain(armPos, armMin, armMax);
}
