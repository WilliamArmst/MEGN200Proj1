//include the necessary libraries for the RF24 Tx/Rx
//Tx = transmitter
//Rx = reciever
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// define struct
// #include "motorDemoStruct.hpp" // ToDo: find proper directory

struct DataPacket {
	int speed = 0;
	int incrementServo = false; 
};


#define REVERSE_BUTTON 2
#define SPEED_PIN A0

//create the RF24 object
RF24 radio(9, 8);  // CE, CSN
DataPacket data;

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
  data.speed =analogRead(SPEED_PIN); // double check syntax is correct
  data.incrementServo = !digitalRead(REVERSE_BUTTON);


  radio.write(&data, sizeof(DataPacket)); //transmit data

  Serial.print("Packet Sent: {speed: ");
  Serial.print(data.speed);
  Serial.print(", incrementServo: ");
  Serial.print(data.incrementServo);
  Serial.println("}");

  delay(50); // transmission delay
}
