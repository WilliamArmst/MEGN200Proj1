//same libraries and object creation as your transmitter
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 8);  // CE, CSN

//Yes you have to have same structure variable 'DataPacket' 
struct DataPacket {

  //THIS IS YOUR TRANSMISSION VARIABALE INITIALIZATION BLOCK
  
  //Yes, this is and must be the same block from your transmitter code
  //Again, yes, you must have the same transmission variables...
  //  In the reciever code as you do in the transmitter code
  bool TransmitTheBool;//character variable

  //THIS IS YOUR TRANSMISSION VARIABALE INITIALIZATION BLOCK

};

//check transmit_demo if confused - all of this is the same
DataPacket data; //same structure variables 'DataPacket and 'data' as transmitter

//address through which two modules communicate.
const byte address[6] = "34768";


void setup() {
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(0, address);//set receiving to 5-digit address
  radio.startListening();//pretty explicit code call
  //"Sometimes I wish my students had this built-in function" - Prof. Kuska circa 2024
  Serial.begin(9600);
}

void loop() {
  //delay(20);//20ms delay to get signal
  if (radio.available()) {//if a signal is available
    radio.read(&data, sizeof(DataPacket));//read signal being sent
    //the data structure is now here - weeeeeeeeeeeee
    delay(20);//20ms delay to unpack signal

    //we can now use variables in this void loop(). SICK!
    Serial.println(data.TransmitTheBool);//print that variable we created and transmitted
    //what do you think it will print?!

  }
}
