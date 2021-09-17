/*
* Arduino Wireless Communication Tutorial
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

Servo elevator,rudder,ael_rt,ael_lt,motor;  // create servo object to control a servo


RF24 radio(7, 8); // CE-9, CSN-10 for nano, I am using 7,8 as i need more pwn pins
bool rc;
const byte address[6] = "000A1";

// Max size of this struct can be 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte joy1X=90;
  byte joy1Y=90;
  byte joy2X=90;
  byte joy2Y=90;
  byte pot1=0;
  byte pot2=0;
  bool tSwitch1;
  bool tSwitch2;
  bool tSwitch3;
  bool buttonu;
  bool buttond;
  bool buttonr;
  bool buttonl;
};

Data_Package data; //Create a variable with the above structure


void setup() {
  Serial.begin(9600);
  
  elevator.attach(5);  // attaches the elevtor servo on pin 5 to the servo object
  rudder.attach(6);    // attaches the rudder servo on pin 6 to the servo object
  ael_rt.attach(9);    // attaches the ael_rt servo on pin 9 to the servo object
  ael_lt.attach(10);   // attaches the ael_lt servo on pin 10 to the servo object
  motor.attach(3,1000,2000);  // attaches the bldc motor on pin 3 to the servo object
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  //Check is nrf24l01 is connected properly: 1->yes
  rc = radio.isChipConnected ();
  Serial.println (rc);
}

void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package));
//    Serial.print(data.joy1X);
//    Serial.print(data.joy1Y);
//    Serial.print(data.joy2X);
//    Serial.print(data.joy2Y);
//    Serial.print(data.pot1);
//    Serial.print(data.pot2);
//    Serial.print(data.tSwitch1);
//    Serial.print(data.tSwitch2);
//    Serial.print(data.tSwitch3);
//    Serial.print(data.buttonu);
//    Serial.print(data.buttond);
//    Serial.print(data.buttonr);
//    Serial.print(data.buttonl);
//    bool result = radio.isChipConnected ();
    Serial.println (rc);
    motor.write(data.joy1Y);
//    elevator.write(data.joy2Y);
//    rudder.write(data.joy2X);
//    delay(15);
  }
}
