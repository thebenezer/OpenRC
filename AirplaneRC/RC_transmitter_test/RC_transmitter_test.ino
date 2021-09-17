
//Code to get joystick values and display it on OLED 128*64
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define joy1X A0
#define joy1Y A1
#define joy2X A2
#define joy2Y A3
#define pot1 A6
#define pot2 A7
#define sw1 6
#define sw2 5
#define sw3 4
#define bup 2
#define bdn 7
#define brt 3
#define blt 8
RF24 radio(7, 8); // CE, CSN

const byte address[6] = "000A1";
bool result ;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  Serial.begin(9600);
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  pinMode(sw3, INPUT_PULLUP);
  pinMode(bup, INPUT_PULLUP);
  pinMode(bdn, INPUT_PULLUP);
  pinMode(brt, INPUT_PULLUP);
  pinMode(blt, INPUT_PULLUP);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
  //Check is nrf24l01 is connected properly: 1->yes
  result = radio.isChipConnected ();

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(500); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
}

void loop() {
//    testdrawchar();      // Draw characters of the default font
//    testdrawstyles();
//    testscrolltext();
 
  //print the values to display
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Joy1"));
  display.print(analogRead(joy1X)); //X pos of joystick
  display.print("-");
  display.println(analogRead(joy1Y)); //Y pos of joystick
  display.println(F("Joy2"));
  display.print(analogRead(joy2X)); //X pos of joystick
  display.print("-");
  display.println(analogRead(joy2Y));
  display.print("-");
  display.print(F("Pot1: "));
  display.println(analogRead(pot1)); //X pos of joystick
  display.print(F("Pot2: "));
  display.println(analogRead(pot2));

  display.setCursor(80,10);             // switches and buttons
  display.println(F("Switches"));
  display.setCursor(80,20);
  display.print(digitalRead(sw1));
  display.print(digitalRead(sw2));
  display.print(digitalRead(sw3));
  display.setCursor(80,30);
  display.println(F("Buttons"));
  display.setCursor(80,40);
  display.print(digitalRead(bup));
  display.print(digitalRead(bdn));
  display.print(digitalRead(brt));
  display.print(digitalRead(blt));
  
  //Finally call the display function to render the content  
  display.display();
}
