
/*
  Arduino Wireless Communication Tutorial
      Example 1 - Transmitter Code

  by Dejan Nedelkovski, www.HowToMechatronics.com

  Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00AA1";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
  //Check is nrf24l01 is connected properly: 1->yes
  bool result = radio.isChipConnected ();
  Serial.println (result);
}

void loop() {
  const char text[] = "Hello bee";
  radio.write(&text, sizeof(text));
  Serial.print("Transmiting");
  Serial.println (text);
  delay(1000);
}