//same libraries and object creation as your transmitter
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// #include "../includes/motorDemoStruct.h"

struct DataPacket {
	int speed = 0;
	int isReversed = false; 
};

// define pins
#define ENA_1 5 // enable1
#define DRIVE_1A 4 // in1
#define DRIVE_2A 3 // in2

RF24 radio(9, 8);  // CE, CSN
DataPacket data; //same structure variables 'DataPacket and 'data' as transmitter

bool in1State = LOW;
bool in2State = LOW;

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
}

void loop() {
  if (radio.available()) {//if a signal is available
    radio.read(&data, sizeof(DataPacket));//read signal being sent
    delay(20);//20ms delay to unpack signal
  }

  if (data.isReversed) {
    in1State = LOW;
  } else {
    in1State = HIGH;
  }

  if (data.speed < 3) {// 0.8%
    in1State = LOW;
    in2State = LOW;
    data.speed = 0;
  } else {
    in2State = HIGH;
  }

  digitalWrite(DRIVE_1A, in1State);
  digitalWrite(DRIVE_1A, in1State);
  analogWrite(ENA_1, data.speed);
}
