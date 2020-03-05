#include <FastLED.h>
#include <SoftwareSerial.h>

#define NUM_LEDS 100         // number of LEDs
#define DATA_PIN 7

int led = 7;
int tx = 1;
int rx = 0;
char inSerial[15];

CRGB leds[NUM_LEDS];          // Sets upp an array to set/clear led data
#define FRAMES_PER_SECOND  120
uint8_t gHue = 0;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(tx, OUTPUT);
  pinMode(rx, INPUT);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
      //SoftwareSerial myserial(;
      //myserial.begin();

}



void loop() {

  
  int i = 0;
  delay(500);
  if (Serial.available() > 0 ) {
    while (Serial.available() > 0) {
      inSerial[i] = Serial.read();
      i++;
    }
    inSerial[i] = '\0';
    Check_protocol(inSerial);
  }

}

void Check_protocol(char inStr[]) {
  int i = 0;
  int m = 0;
  int r = 0;
  int RED_Value = 0;
  int GREEN_Value = 0;
  int BLUE_Value = 0;
  float LED_Brightness = 0;
  Serial.println(inStr);

//----------------------------------------------------------------SetBrightness (not working properly-------------------

  if (!strcmp(inStr[0], 'L')) {
    float Brightness_Value = 0;

    Brightness_Value += ((inStr[1] - 48) * 100);
    Brightness_Value += ((inStr[2] - 48) * 10);
    Brightness_Value += inStr[3] - 48;

    LED_Brightness = (Brightness_Value / 255);
    Serial.print("Brightness = ");
    Serial.println(Brightness_Value);
    Serial.print("Brightness % = ");
    Serial.println(LED_Brightness);
    Brightness_Value = 0;
  }

//-----------------------------------------------------------------Colorwheel----------------------------------------------


  if (!strcmp(inStr[0], 'G')) {

    RED_Value += ((inStr[1] - 48) * 100);
    RED_Value += ((inStr[2] - 48) * 10);
    RED_Value += inStr[3] - 48;
    Serial.print("R = ");
    Serial.print(RED_Value);

    GREEN_Value += ((inStr[4] - 48) * 100);
    GREEN_Value += ((inStr[5] - 48) * 10);
    GREEN_Value += inStr[6] - 48;
    Serial.print(" / G = ");
    Serial.print(GREEN_Value);

    BLUE_Value += ((inStr[7] - 48) * 100);
    BLUE_Value += ((inStr[8] - 48) * 10);
    BLUE_Value += inStr[9] - 48;
    Serial.print(" / B = ");
    Serial.println(BLUE_Value);


    for (int v = 0; v < NUM_LEDS; v++) {
      leds[v].setRGB( (RED_Value * LED_Brightness), (GREEN_Value * LED_Brightness), (BLUE_Value * LED_Brightness));

    }
    FastLED.show();
  }


//--------------------------------------------------------------BLUE------------------------------------------------

  if (!strcmp(inStr, "BLUE")) {           // Set led 1 blue
    for (r = 0; r < (NUM_LEDS - 1); r++) {
      leds[r] = CRGB::Blue;
    }
    r = 0;
    FastLED.show();
    for (m = 0; m < 14; m++) {
      inStr[m] = 0;
    }
    i = 0;
  }

  //-------------------------------------------------------------RED-------------------------------------------------

  if (!strcmp(inStr, "RED")) {           // Set led 1 red
    for (r = 0; r < (NUM_LEDS - 1); r++) {
      leds[r] = CRGB::Red;
    }
    r = 0;
    FastLED.show();
    for (m = 0; m < 14; m++) {
      inStr[m] = 0;
    }
    i = 0;
  }

//------------------------------------------------------------Confetti-----------------------------------------------

  while (!strcmp(inStr, "CON")){
      EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
    // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 
  
      fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
  //////////////////////////////////////////////////////////
   int i = 0;
  if (Serial.available() > 0 ) {
    while (Serial.available() > 0) {
      inSerial[i] = Serial.read();
      i++;
    }
    inSerial[i] = '\0';
    Check_protocol(inSerial);
  }
  }

  //-----------------------------------------------------------Rainbow------------------------

   while (!strcmp(inStr, "RAIN")) {
    
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);

        EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
    // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

     int i = 0;
  if (Serial.available() > 0 ) {
    while (Serial.available() > 0) {
      inSerial[i] = Serial.read();
      i++;
    }
    inSerial[i] = '\0';
    Check_protocol(inSerial);
   }
   }

//---------------------------------------------------------------BPM----------------------

while (!strcmp(inStr, "BPM")) {

  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 128;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }

      // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 


      int i = 0;
  if (Serial.available() > 0 ) {
    while (Serial.available() > 0) {
      inSerial[i] = Serial.read();
      i++;
    }
    inSerial[i] = '\0';
    Check_protocol(inSerial);
   }
  
}

//--------------------------------------------------------------------JUGGLE-------------------------------

while (!strcmp(inStr, "JUG")) {

  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }

       // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

        int i = 0;
  if (Serial.available() > 0 ) {
    while (Serial.available() > 0) {
      inSerial[i] = Serial.read();
      i++;
    }
    inSerial[i] = '\0';
    Check_protocol(inSerial);
   } 

  
}

//-------------------------------------------------------------------------------------------------

  
  /*
    if(!strcmp(inStr, "ON")){
    digitalWrite(led, HIGH);
    for(m=0; m<14;m++){
      inStr[m]=0;
    }
    i=0;
    }
    if(!strcmp(inStr, "OFF")){
    digitalWrite(led, LOW);
    for(m=0; m<14;m++){
      inStr[m]=0;
    }
    i=0;
    }
  */

}
