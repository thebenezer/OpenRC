/*
* Arduino Wireless Communication Test
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE-9/7, CSN-10/8 
bool result;
const byte address[6] = "00AA1";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  //Check is nrf24l01 is connected properly: 1->yes
  bool result = radio.isChipConnected ();
  Serial.println (result);
}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
    bool result = radio.isChipConnected ();
    Serial.println (result);
  }
}
