// Modified by @koppanyh from the following:
// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>

const int onboard_led_pin = 17;

const int neopixel_pin = 10;
const int neopixel_len = 3;

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(neopixel_len, neopixel_pin, NEO_GRB + NEO_KHZ800);

int key_pins[] = {2, 3, 4, 5, 6, 7, 8, 9};
const int keys_len = sizeof(key_pins) / sizeof(int);
const int button_pin = 15;
const int encoder_clk = 14;
const int encoder_data = 16;

const uint32_t black = pixels.Color(0, 0, 0);
int brightness = 1;
uint32_t colors[] = {
  pixels.Color(1, 0, 0),
  pixels.Color(0, 1, 0),
  pixels.Color(0, 0, 1)
};
const int colors_len = sizeof(colors) / sizeof(uint32_t);
int selected_color = 1;
void InitColors() {
  colors[0] = pixels.Color(brightness, 0, 0);
  colors[1] = pixels.Color(0, brightness, 0);
  colors[2] = pixels.Color(0, 0, brightness);
}

int currentStateCLK;
int lastStateCLK;

void setup() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  
  pinMode(onboard_led_pin, OUTPUT);  // init onboard LED
  digitalWrite(onboard_led_pin, LOW);  // onboard LED won't light without this for some reason, and it's inverted...
  delay(500);
  digitalWrite(onboard_led_pin, LOW);

  BootAnim();

  for (int i = 0; i < keys_len; i++)
    pinMode(key_pins[i], INPUT_PULLUP);
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(encoder_clk, INPUT_PULLUP);
  pinMode(encoder_data, INPUT_PULLUP);

  // Read the initial state of CLK
  lastStateCLK = digitalRead(encoder_clk);
}

void BootAnim() {
  digitalWrite(onboard_led_pin, LOW);
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(1, 0, 0));
  pixels.show();
  delay(500);
  pixels.setPixelColor(1, pixels.Color(0, 1, 0));
  pixels.show();
  delay(500);
  pixels.setPixelColor(2, pixels.Color(0, 0, 1));
  pixels.show();
  delay(500);
  digitalWrite(onboard_led_pin, HIGH);
}

void BinaryDisp(uint8_t num) {
  pixels.clear(); // Set all pixel colors to 'off'
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i = 0; i < neopixel_len; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    if (num & 1)
      pixels.setPixelColor(i, colors[selected_color]);
    else
      pixels.setPixelColor(i, black);
    num >>= 1;
  }
  pixels.show();   // Send the updated pixel colors to the hardware.
}

void loop() {
  // Show which key is pressed
  BinaryDisp(0);
  for (int i = 0; i < keys_len; i++) {
    if (!digitalRead(key_pins[i]))
      BinaryDisp(i == 0 ? 7 : i);
  }
  
  // Use the encoder button to rotate through colors
  if(!digitalRead(button_pin)) {
    selected_color++;
    if (selected_color >= colors_len)
      selected_color = 0;
    delay(100);
  }
  
  // Encoder
  for (int i = 0; i < 100; i++) {
    // Read the current state of CLK
    currentStateCLK = !digitalRead(encoder_clk);
    // If last and current state of CLK are different, then pulse occurred
    // React to only 1 state change to avoid double count
    if (currentStateCLK != lastStateCLK  && currentStateCLK){
      // If the DT state is different than the CLK state then
      // the encoder is rotating CCW so decrement
      if (!digitalRead(encoder_data) != currentStateCLK) {
        brightness --;
      } else {
        // Encoder is rotating CW so increment
        brightness ++;
      }
      brightness = max(min(255, brightness), 0);
      InitColors();
    }
    // Remember last CLK state
    lastStateCLK = currentStateCLK;
    delay(1);
  }
  
  //delay(100);
}
