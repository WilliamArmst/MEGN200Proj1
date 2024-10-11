//include the necessary libraries for the RF24 Tx/Rx
//Tx = transmitter
//Rx = reciever
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// define struct
#include "../includes/motorDemoStruct.h" // ToDo: find proper directory

#define REVERSE_BUTTON 2
#define SPEED_PIN A0

//create the RF24 object
RF24 radio(9, 8);  // CE, CSN
DataPacket data;
bool pressed = false;

const byte address[6] = "34768";

void setup() {
  radio.begin();//Tell the RF library to start radio communication
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address);//set transmission address to 5-digit number above
  //Set module as transmitter
  radio.stopListening();
  Serial.begin(9600);//you better know what this does

  pinMode(REVERSE_BUTTON, INPUT_PULLUP);
}

void loop() {
  data.speed = map(analogRead(SPEED_PIN), 0, 1023, 0, 255); // double check syntax is correct

  // true toggle
  if (digitalRead(REVERSE_BUTTON) == LOW && !pressed) {
    data.isReversed = !data.isReversed;
    pressed = true;
  }
  if (digitalRead(REVERSE_BUTTON) == HIGH && pressed) {
    pressed = false;
  }

  Serial.print("In Reverse: ");
  Serial.println(data.isReversed);
  Serial.print("Speed: ");
  Serial.println(data.speed);

  radio.write(&data, sizeof(DataPacket)); //transmit data

  delay(50); // transmission delay
}
