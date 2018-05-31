#include <FastLED.h>

//analog inputs
#define POT1 A1
#define POT2 A2
#define BTN 2

//led strip
#define LED_PIN 5
#define NUM_LEDS 97
#define LED_TYPE  WS2811
#define COLOR_ORDER GRB

//modes
#define HUE 101
#define RAINBOW 102
#define LEDS_ON 103 //TODO
#define SHIFT 104 //TODO
#define GOTO101 105 //last mode, wraps back to 101


CRGB leds[NUM_LEDS];

int pot1 = analogRead(POT1);
int pot2 = analogRead(POT2);
byte btn = digitalRead(BTN);
byte mode = 102;
unsigned long dT = 0;

#define DEFAULT_BRIGHTNESS 31

void setup() {
  Serial.begin(9600);
  delay( 1000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);
}


void loop() {
  int p1 = pot1 - analogRead(POT1);
  int p2 = pot2 - analogRead(POT2);
  if (abs(p1) > 5 || abs(p2) > 5) {

    Serial.print("P1: ");     Serial.println(p1);
    Serial.print("P2: ");     Serial.println(p2);
    Serial.print("Pot 1: ");  Serial.println(analogRead(POT1));
    Serial.print("Pot 2: ");  Serial.println(analogRead(POT2));
    Serial.println();

    ledding(pot1, pot2);
  }
  // if (digitalRead(BTN) == LOW && bounceChek()) Serial.println("blip");

}

byte getRainbowHueOffset(byte ledsOn) {
  byte hueOffset = 255 / ledsOn + map(pot2, 0, 1023, 0, 255);

  if (hueOffset < 2 || hueOffset > 254) {
    return 253;
  }

  return hueOffset;
}

void modHue(byte pot2, byte ledsOn) {
  byte hue = map(pot2, 0, 1023, 0, 255) - 25;

  for (int i = 0; i < ledsOn; i++) {
    leds[i].setHue(hue);
  }
}

void modRainbow(byte pot2, byte ledsOn) {
  if (pot2 < 3 ) pot2 = 3;

  byte hueOffset = getRainbowHueOffset(ledsOn);
  Serial.println(hueOffset);

  byte hue = 0;
  for (int i = 0; i < ledsOn; i++) {
    hue += hueOffset;
    leds[i].setHue(hue);
  }
}

void ledding(int pot1, int pot2) {
  FastLED.clear();
  byte ledsOn = NUM_LEDS; // map(pot1, 0, 1023, 0, NUM_LEDS);

  if (mode == HUE) {
    modHue(pot2, ledsOn);
  } else if (mode == RAINBOW) {
    modRainbow(pot2, ledsOn);
  }

  byte brightness = map(pot1, 0, 1023, 0, 255);
  FastLED.setBrightness(brightness);
  FastLED.show();
}


bool bounceChek(){
  if (millis() - dT > 50) {
    Serial.println(millis() - dT);
    dT = millis();
    return true;
  }
  dT = millis();
  return false;
}
