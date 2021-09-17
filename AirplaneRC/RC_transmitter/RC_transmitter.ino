/*
        DIY Arduino based RC Transmitter
  Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


// Define the digital inputs
#define j1X A1
#define j1Y A0
#define j2X A3
#define j2Y A2

#define t1 6   // Toggle switch 1
#define t2 5   // Toggle switch 1
#define t3 4   // Toggle switch 1
#define bu 2   // Button UP
#define bd 7   // Button DOWN
#define br 3   // Button RIGHT
#define bl 8   // Button LEFT

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "000A1";
bool rc;

//SCREEN WIDTH = 128  OLED display width, in pixels
//SCREEN HEIGHT= 64 OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

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
  
  // Activate the Arduino internal pull-up resistors
  pinMode(t1, INPUT_PULLUP);
  pinMode(t2, INPUT_PULLUP);
  pinMode(t3, INPUT_PULLUP);
  pinMode(bu, INPUT_PULLUP);
  pinMode(bd, INPUT_PULLUP);
  pinMode(br, INPUT_PULLUP);
  pinMode(bl, INPUT_PULLUP);
  
  // Define the radio communication
  radio.begin();
  radio.openWritingPipe(address);
//  radio.setAutoAck(false);
//  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
  //Check is nrf24l01 is connected properly: 1->yes
  rc = radio.isChipConnected ();
  Serial.println(rc);
  
  //initialize the display
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(500); // Pause 
  display.clearDisplay();// Clear the buffer
}

void loop() {
  // Read all analog inputs and map them to one Byte value
  // Convert the analog read value from 0 to 1023 into a BYTE value from 0 to 180
  Serial.println(analogRead(A7));
  Serial.println(analogRead(A6));
  data.pot1   = map(analogRead(A7), 0, 1023, 0, 180) ;//Throws
  data.pot2   = map(analogRead(A6), 0, 1023, 0, 180) ;

  data.joy1X = 90;                                   //Aeleron
  data.joy1Y = map(analogRead(j1Y), 0, 1023, 0, 180);//Thrust
  data.joy2X = map(analogRead(j2X), 0, 1023, 90-data.pot1, 90+data.pot1);//Rudder
  data.joy2Y = map(analogRead(j2Y), 0, 1023, 90-data.pot1, 90+data.pot1);//Elevator
  
  // Read all digital inputs
  data.tSwitch1 = digitalRead(t1);
  data.tSwitch2 = digitalRead(t2);
  data.tSwitch3 = digitalRead(t3);
  data.buttonu = digitalRead(bu);
  data.buttond = digitalRead(bd);
  data.buttonr= digitalRead(br);
  data.buttonl = digitalRead(bl);
  

  
  
  // If toggle switch 2 is switched on
  if (digitalRead(t2) == 0) {
    data.joy1X = map(analogRead(j1X), 1023, 0, 0, 180); // Use on Aelron Control
  }
    // If toggle switch 1 is switched on
  if (digitalRead(t1) == 0) {
    OLEDtest();    // Turn on display for testing
  }
  
  // Send the whole data from the structure to the receiver
  radio.write(&data, sizeof(Data_Package));
}

void OLEDtest() {
  
  
  //Check is nrf24l01 is connected properly: 1->yes
  rc = radio.isChipConnected ();
  //print the values to display
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Joy1"));
  display.print(data.joy1X); //X pos of joystick1
  display.print("-");
  display.println(data.joy1Y); //Y pos of joystick1
  display.println(F("Joy2"));
  display.print(data.joy2X); //X pos of joystick2
  display.print("-");
  display.println(data.joy2Y);//X pos of joystick2
  
  display.print(F("Pot1: "));
  display.println(data.pot1); //pot 1 value
  display.print(F("Pot2: "));
  display.println(data.pot2);//pot 1 value
  display.setCursor(40,0);             // rc address
  display.print(char(address[0]));
  display.print(char(address[1]));
  display.print(char(address[2]));
  display.print(char(address[3]));
  display.println(char(address[4]));
  display.setCursor(80,0);             // radio is connected or not
  display.println(rc);
  display.setCursor(80,10);             // switches and buttons
  display.println(F("Switches"));
  display.setCursor(80,20);
  display.print(digitalRead(t1));
  display.print(digitalRead(t2));
  display.print(digitalRead(t3));
  display.setCursor(80,30);
  display.println(F("Buttons"));
  display.setCursor(80,40);
  display.print(digitalRead(bu));
  display.print(digitalRead(bd));
  display.print(digitalRead(br));
  display.print(digitalRead(bl));
  
  //Finally call the display function to render the content  
  display.display();
}
