#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define RIGHT_X A2
#define RIGHT_Y A3
#define RIGHT_PUSH 3
#define LEFT_X A0
#define LEFT_Y A1
#define LEFT_PUSH 2

struct DataPacket {
  int rightX = 0;
  int rightY = 0;
  int rightPress = 0;
  int leftX = 0;
  int leftY = 0;
  int leftPress = 0;
};

// joystick values are from 0 to 1024
// middle is about 512

RF24 radio(9, 8);  // CE, CSN
DataPacket data;
const byte address[6] = "34768";

void setup() {
  // setup radio
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address);
  radio.stopListening();

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  data.rightX = analogRead(RIGHT_X);
  data.rightY = analogRead(RIGHT_Y);
  data.leftX = analogRead(LEFT_X);
  data.leftY = analogRead(LEFT_Y);
  data.leftPress = digitalRead(LEFT_PUSH);
  data.rightPress = digitalRead(RIGHT_PUSH);

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

  radio.write(&data, sizeof(DataPacket));//transmit data
}
