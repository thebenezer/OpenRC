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


// Define the analog inputs
#define j1X A1
#define j1Y A0
#define j2X A3
#define j2Y A2
#define p1 A7
#define p2 A6

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
int ele_throws,rudd_throws,ele_trim=0,rudd_trim=0;

//SCREEN WIDTH = 128  OLED display width, in pixels
//SCREEN HEIGHT= 64 OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

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
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
  
  //Check if nrf24l01 is connected properly: 1->yes
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
//  Serial.println(analogRead(A7));
//  Serial.println(analogRead(A6));
  ele_throws   = map(analogRead(p1), 0, 1023, -90, 90);//Elevator throws
  rudd_throws   = map(analogRead(p2), 0, 1023, -90, 90) ;//Rudder throws

  data.thrust = 0;//Thrust  
  data.ail = data.ail = map(analogRead(j1X), 1023, 0, 90-20, 90+20); // Aileron
  data.rudd = rudd_trim+map(analogRead(j2X), 0, 1023, 90-rudd_throws, 90+rudd_throws);//Rudder
  data.elev = ele_trim+map(analogRead(j2Y), 0, 1023, 90-ele_throws, 90+ele_throws);//Elevator

  // Switches to limit thrust
  if (digitalRead(t3) == 0) { // If toggle switch 3 is switched on use 100% thrust
    data.thrust = map(analogRead(j1Y), 0, 1023, 0, 180);
  }
  else if (digitalRead(t2) == 0) { // If toggle switch 2 is switched on use thrust, a safety measure to prevent accidental thrust input :P
    data.thrust = map(analogRead(j1Y), 0, 1023, 0, 90);
  }
  
  // If toggle switch 1 is switched on Turn on display for testing
  if (digitalRead(t1) == 0) {
    OLEDtest();   
    
    // Using Push buttons for trim
    if(digitalRead(bu) == 0){
      ele_trim+=1;
    }
    else if(digitalRead(bd) == 0){
      ele_trim-=1;
    }
    else if(digitalRead(br) == 0){
      rudd_trim+=1;
    }
    else if(digitalRead(bl) == 0){
      rudd_trim-=1;
    }
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
  display.print(data.ail); //X pos of joystick1
  display.print("-");
  display.println(data.thrust); //Y pos of joystick1
  display.println(F("Joy2"));
  display.print(data.rudd); //X pos of joystick2
  display.print("-");
  display.println(data.elev);//X pos of joystick2
  
  display.print(F("e_throws: "));
  display.println(ele_throws); //pot 1 value
  display.print(F("r_throws: "));
  display.println(rudd_throws);//pot 1 value
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