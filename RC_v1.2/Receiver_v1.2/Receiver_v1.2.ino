/*
* Arduino Wireless Communication Tutorial
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

Servo elevator,rudder,ail_rt,ail_lt,motor;  // create servo object to control a servo

unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

RF24 radio(7, 8); // CE-9, CSN-10 for nano, I'm using 7,8 as I need more pwn pins(i.e,9,10)
const byte address[6] = "000A1";

// Max size of this struct can be 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte ail=90;
  byte thrust=0;
  byte rudd=90;
  byte elev=90;
};

Data_Package data; //Create a variable with the above structure


void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  elevator.attach(5);  // attaches the elevtor servo on pin 5 to the servo object
  rudder.attach(6);    // attaches the rudder servo on pin 6 to the servo object
  ail_rt.attach(9);    // attaches the ael_rt servo on pin 9 to the servo object
  ail_lt.attach(10);   // attaches the ael_lt servo on pin 10 to the servo object
  motor.attach(3,1000,2000);  // attaches the bldc motor on pin 3 to the servo object
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
//  tone(2, 1000); // Send 1KHz sound signal...
//    delay(500);
//    noTone(2);
}

void loop() {
  currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetData(); // If connection is lost, reset the data.
  }
  if (radio.available()) {
    lastReceiveTime = millis(); // At this moment we have received the data
    radio.read(&data, sizeof(Data_Package));
//    tone(2, 1000); // Send 1KHz sound signal...
//    delay(500); noTone(2);delay(500);
    
    elevator.write(data.elev);
    rudder.write(data.rudd);
    ail_rt.write(data.ail);
    ail_lt.write(data.ail);
    motor.write(data.thrust);
  }
}

void resetData(){
  motor.write(0);//turn off thrust to prevent plane from flying away
}