//include the necessary libraries for the RF24 Tx/Rx
//Tx = transmitter
//Rx = reciever
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create the RF24 object
RF24 radio(9, 8);  // CE, CSN
//this object is recognized by the RF24 library
//and takes the two pin arguments for the CE and CSN pin
//CE is basically the input pin and is used to control data transmission
//CSN pin is a chip selection pin and turns on and off to recieve or transmit signal
//if you followed the provided wiring diagram then you do not need to change these

bool TransmitTheBool = 0; //boolean variable initialization as 0
//Will add to data packet which is what we transmit below

//a structure is a variable that can hold more variables
//think of it like a filing cabnet with as many files in it as you want
//where each file is a different variable packaged into the larger structure DataPacket
struct DataPacket {

  //THIS IS YOUR TRANSMISSION VARIABALE INITIALIZATION BLOCK

  //THIS is where you will initialize all variables for the control of the CoLeBot
  //You MUST initialize a variable corresponding to each sensor value you decide to use
  //Examples inlcude: a joystick's x and y value and its button, or another button or sensor of your choosing
  //Here to show an example I am creating a boolean variable
  bool TransmitTheBool;//character variable

  //THIS IS YOUR TRANSMISSION VARIABALE INITIALIZATION BLOCK

};

//assign a variable to hold all of the desired data to be transmitted
//in this case our variable is called 'data'
DataPacket data;

//create a unique 'address' for your tx and rx
//this needs to be unique and the same between both your tx and rx
//if there are many with the same number there will be cross-talk
//if they aren't matching... guess what happens
//transmitted data is identified by this 5-digit pin, so rx's will only look for data with this identifier
const byte address[6] = "34768";
//you can change the 5-digit number but do not change the variable initialization or name
//those are necessary for the RF24 library

void setup() {
  radio.begin();//Tell the RF library to start radio communication
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address);//set transmission address to 5-digit number above
  //Set module as transmitter
  radio.stopListening();
  Serial.begin(9600);//you better know what this does
}

void loop() {

  //THIS IS YOUR VARIABALE ALLOCATION BLOCK

  //This is where you will allocate values to the initialized variables in your DataPacket STRUCTure
  //YOU WILL NEED TO DO THIS FOR EACH SENSOR READING
  //TL;DR - read each sensor and save its readings to an initialized variables above
  data.TransmitTheBool = !data.TransmitTheBool;// change the boolean value of our variable and then assign it to that variable in the data structure
  //saving that  variable into our data variable which is in our DataPacket structure

  //THIS IS the end of YOUR VARIABALE ALLOCATION BLOCK

  Serial.println(data.TransmitTheBool);//print the variable were transmitting here
  //not necessary but just for funsies
  radio.write(&data, sizeof(DataPacket));//transmit data
  //here we are transmitting the structure 'data' within the strucure 'DataPacket'
  delay(50);//chill out for like 20 ms...NEED THIS FOR TRANSMISSION DELAY
}
