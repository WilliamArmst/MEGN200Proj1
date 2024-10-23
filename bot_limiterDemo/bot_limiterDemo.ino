// #include <SPI.h>
// #include <nRF24L01.h>
// #include <RF24.h>
#include <Servo.h>

// servos
#define ARM_SERVO A0
#define CLAMP_SERVO A1

Servo armServo;
Servo clampServo;

// 0 to 180
int armPos = 90;
int clampPos = 90;

const int minArmPos = 63; // arm is theoretically 50-150
const int maxArmPos = 145;
const int minClampPos = 65; // clamp is theoretically 50-140
const int maxClampPos = 136;

void setup() {
  Serial.begin(9600);
  
  // set pins to output
  pinMode(ARM_SERVO, OUTPUT);
  pinMode(CLAMP_SERVO, OUTPUT);

  armServo.attach(ARM_SERVO);
  clampServo.attach(CLAMP_SERVO);
}

void loop() {
  // armPos += 1; // test max arm
  // armPos -= 1; // test min arm
  if (armPos > maxArmPos) {
    armPos = maxArmPos;
  }
  if (armPos < minArmPos) {
    armPos = minArmPos;
  }
  armServo.write(armPos);

  // clampPos += 1; // test max clamp
  // clampPos -= 1; // test min clamp
  if (clampPos > maxClampPos) {
    clampPos = maxClampPos;
  }
  if (clampPos < minArmPos) {
    clampPos = minClampPos;
  }
  clampServo.write(clampPos);

  delay(3);

}
