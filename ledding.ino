#include <FastLED.h>


#define POT1 A1
#define POT2 A2
#define BTN 2

#define LED_PIN 5
#define NUM_LEDS 97
#define LED_TYPE  WS2811
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

int pot1 = analogRead(POT1);
int pot2 = analogRead(POT2);
int btn = digitalRead(BTN);
unsigned long pressTime = 0;

byte hue = 0;
int ledsOn = 0;

void setup() {
  Serial.begin(9600);
  delay( 1000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(31);
}


void loop() {
  
  if (abs(pot1 - analogRead(POT1)) > 5 || abs(pot2 - analogRead(POT2)) > 5 ) potReader();
  if (digitalRead(BTN) == LOW && bounceChek()) Serial.println("blip");
  FastLED.show();
//  FastLED.delay(50);
  if(millis() % 400 < 10 ) hue++;
}

void potReader() {
  pot1 = analogRead(POT1);
  pot2 = analogRead(POT2);

  ledding();
}

void ledding() {
  FastLED.clear();
  ledsOn = map(pot1, 0, 1023, 0, NUM_LEDS);
  hue = map(pot2, 0, 1023, 0, 255)-25;
  
  for (int i = 0; i < ledsOn; i++) leds[i].setHue(hue);
}


bool bounceChek(){
  if (millis() - pressTime > 50) {
    Serial.println(millis() - pressTime);
    pressTime = millis();
    return true;
  }
  pressTime = millis();
  return false;
}

