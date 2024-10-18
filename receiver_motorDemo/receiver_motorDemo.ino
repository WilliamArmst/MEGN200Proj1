//same libraries and object creation as your transmitter
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

// #include "../includes/motorDemoStruct.h"

struct DataPacket {
	int speed = 0;
	int incrementServo = false; 
};

// define pins
#define ENA_1 5 // enable1
#define DRIVE_1A 4 // in1
#define DRIVE_2A 3 // in2

RF24 radio(9, 8);  // CE, CSN
DataPacket data; //same structure variables 'DataPacket and 'data' as transmitter
Servo myServo;

bool in1State = LOW;
bool in2State = LOW;

int servoPos = 0;
int reverseServo = 0;

//address through which two modules communicate.
const byte address[6] = "34768";


void setup() {
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(0, address);//set receiving to 5-digit address
  radio.startListening(); // set as receiver

  Serial.begin(9600);

  pinMode(ENA_1, OUTPUT);
  pinMode(DRIVE_1A, OUTPUT);
  pinMode(DRIVE_2A, OUTPUT);

  myServo.attach(10);
}

void loop() {
  if (radio.available()) {//if a signal is available
    radio.read(&data, sizeof(DataPacket));//read signal being sent
    delay(20);//20ms delay to unpack signal

    Serial.print("Packet Received: {speed: ");
    Serial.print(data.speed);
    Serial.print(", incrementServo: ");
    Serial.print(data.incrementServo);
    Serial.println("}");
  }

  if (data.incrementServo) {
    if (reverseServo) {
      servoPos -= 1;
    } else {
      servoPos += 1;
    }
    
    if (servoPos >= 180 || servoPos <= 0) {
      reverseServo = !reverseServo;
    }

    myServo.write(servoPos);
  }

  if (data.speed < 512) {
    in1State = HIGH;
    in2State = LOW;
    map(data.speed, 0, 512, 0, 1023);
  } else {
    in1State = LOW;
    in2State = HIGH;
    map(data.speed, 512, 1023, 0, 1023);
  }
  
  // in1State = HIGH; in2State = HIGH; data.speed = 128;
  digitalWrite(DRIVE_1A, in1State);
  digitalWrite(DRIVE_2A, in2State);
  analogWrite(ENA_1, data.speed);
}
