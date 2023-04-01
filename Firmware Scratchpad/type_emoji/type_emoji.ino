#include "Keyboard.h"

int key_pins[] = {2, 3, 4, 5, 6, 7, 8, 9};
const int keys_len = sizeof(key_pins) / sizeof(int);

void setup() {
  for (int i = 0; i < keys_len; i++)
    pinMode(key_pins[i], INPUT_PULLUP);
  Keyboard.begin();
}

bool keypressed(int num) {
  return !digitalRead(key_pins[num]);
}

void key(int num, char* msg) {
  if (keypressed(num)) {
    Keyboard.print(msg);
    delay(200);
  }
}

void loop() {
  key(0, "Hello, World!");
  key(1, "😈");
  if (keypressed(2)) {
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.print("d83dde08");
    Keyboard.releaseAll();
    delay(200);
  }
}
