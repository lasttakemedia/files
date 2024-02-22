#include <Adafruit_NeoPixel.h>

#define PIN            13   // Pin connected to NeoPixel data input
#define NUMPIXELS      1   // Number of NeoPixels
#define SWITCH_PIN     12   // Pin connected to the switch
#define POT_PIN        A6  // Pin connected to the potentiometer

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

unsigned long event = 100;
//unsigned long currentMillis = 0; //stores the current time


const int BtnBlo = A4; // Vent toggle switch
const int ledBlo = A5; // Vent Light
const int BtnBar = A2; 
const int ledBar = A3;

//******************** POWERCELL PRE****************
const int ledPins[] = {6, 7, 8, 9, 10, 11, A0, A1}; // Array to hold the LED pins
const int numLEDs = sizeof(ledPins) / sizeof(ledPins[0]); // Number of LEDs

unsigned long previousMillis = 0; // Store the last time LEDs were updated
unsigned long interval = 150; // Interval between lighting up LEDs

int currentLED = 0; // Variable to track the current LED
bool turnOff = false; // Variable to track if all LEDs should be turned off

//********************* CYC PRE********************
// Define the pins for the LEDs
const int ledPinsCyc[] = {2, 3, 4, 5};
const int numLedsCyc = sizeof(ledPinsCyc) / sizeof(ledPinsCyc[0]);

// Interval between LED changes
const unsigned long intervalCyc = 1200; // in milliseconds

// Variable to store the previous time
unsigned long previousMillisCyc = 0;

// Variable to store the current LED index
int currentLedIndexCyc = 0;

// Variable to track whether all LEDs are lit
bool allLedsLitCyc = false;
//********************* PRE ********************
int ledNum = 0;
int ledNum_brightness = 255;
int ledNum2 = 0;
int ledNum2_brightness = 255;
int btnVentState = HIGH; // Previous state of the fire button
int ledStateVent = LOW;  // ledState used to set the LED
int btnBloState = HIGH; // Previous state of the fire button
int ledStateBlo = LOW;  // ledState used to set the LED
int btnBarState = HIGH; // Previous state of the fire button
int ledStateBar = LOW;  // ledState used to set the LED

//************************* POWERCELL*************************
void Powercell(){
 unsigned long currentMillis = millis(); // Get the current time

  // Check if it's time to update the LEDs
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Update the last time LEDs were updated
    
    // Turn off all LEDs if flag is set
    if (turnOff) {
      for (int i = 0; i < numLEDs; i++) {
        digitalWrite(ledPins[i], LOW);
      }
      turnOff = false; // Reset the flag
      delay(100); // Delay to ensure LEDs are off
    }
    else {
      // Turn on the current LED (and keep the previous ones on)
      for (int i = 0; i < numLEDs; i++) {
        digitalWrite(ledPins[i], i <= currentLED ? HIGH : LOW);
      }

      // Move to the next LED
      currentLED++;

      // If all LEDs are lit, set flag to turn off LEDs
      if (currentLED >= numLEDs) {
        turnOff = true;
        currentLED = 0; // Reset LED index
      }
    }
  }
}
//*************************CYCLOTRON************************
void CYC() {
 // Get the current time
  unsigned long currentMillisCyc = millis();

  // Check if it's time to change the LED
  if (currentMillisCyc - previousMillisCyc >= intervalCyc) {
    // Save the current time for the next iteration
    previousMillisCyc = currentMillisCyc;

    // Turn off the previous LED
    if (currentLedIndexCyc > 0) {
      digitalWrite(ledPinsCyc[currentLedIndexCyc - 1], LOW);
    } else if (currentLedIndexCyc == 0 && allLedsLitCyc) {
      digitalWrite(ledPinsCyc[numLedsCyc - 1], LOW);
    }

    // Turn on the current LED
    digitalWrite(ledPinsCyc[currentLedIndexCyc], HIGH);

    // Move to the next LED
    currentLedIndexCyc++;

    // If we reached the last LED, reset the sequence
    if (currentLedIndexCyc >= numLedsCyc) {
      currentLedIndexCyc = 0;
      allLedsLitCyc = true;
    }
  }
}
//************************** VENT LIGHT ********************
void VentLight() { 
	int switchState = digitalRead(SWITCH_PIN);

  if (switchState == LOW) { // If the switch is pressed
    // Read the potentiometer value
    int potValue = analogRead(POT_PIN);
    
    // Map the potentiometer value (0-1023) to NeoPixel color range (0-255)
    int hue = map(potValue, 0, 9, 0, 255);
    
    // Set NeoPixel color
    pixels.setPixelColor(0, pixels.ColorHSV(hue, 255, 255)); // Set color based on potentiometer value
    pixels.show(); // Display the color
  } else {
    pixels.clear(); // Turn off NeoPixel when the switch is not pressed
    pixels.show();  // Display the off state
  }
	
}
//************************** BAR LIGHT ********************
void BarLight() { 
	
  int pbBar = digitalRead(BtnBar); 

  if(pbBar == LOW && btnBarState == HIGH)
  {
     btnBarState = LOW;
     digitalWrite(ledBar, HIGH);
     //player.playMp3Folder(2);
  }
  else if (pbBar == HIGH && btnBarState == LOW)
  {
      btnBarState = HIGH;
      digitalWrite(ledBar, LOW);
     // player.playMp3Folder(1);
  }
}
//************************** Slo Blo LIGHT ********************
void BloLight() { 

  int pbBlo = digitalRead(BtnBlo); 

  if(pbBlo == LOW && btnBloState == HIGH)
  {
     btnBloState = LOW;
     digitalWrite(ledBlo, HIGH);
     //player.playMp3Folder(2);
  }
  else if (pbBlo == HIGH && btnBloState == LOW)
  {
      btnBloState = HIGH;
      digitalWrite(ledBlo, LOW);
     // player.playMp3Folder(1);
  }
}
//************************** SETUP ***************************
void setup() 
{
	// Inputs/ Outputs
	pinMode (BtnBar, INPUT_PULLUP);
	pinMode (ledBar, OUTPUT);
	pinMode (BtnBlo, INPUT_PULLUP);
	pinMode (ledBlo, OUTPUT);
	pinMode(SWITCH_PIN, INPUT_PULLUP); // Set the switch pin as input with 
	  // Set all LED pins as outputs
   // Initialize all LED pins as outputs
  for (int i = 0; i < numLEDs; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
	// Set all LED pins as outputs
  for (int i = 0; i < numLedsCyc; i++) {
    pinMode(ledPinsCyc[i], OUTPUT);
  }
	
	pixels.begin();
  pixels.clear(); // Turn off NeoPixel initially
  pixels.show();  // Display the initial state
  
}
//************************* LOOP ***************************** 
void loop()
{
//currentMillis = millis();
	CYC();
  Powercell();
	VentLight();
	BloLight();
	BarLight();
}