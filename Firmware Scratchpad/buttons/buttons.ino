// -------------------------------------------------------------------------- //
/* This is the concept dev file for the button system.
 * It includes facilities to debounce buttons, detect single clicks, double
 * clicks, and held buttons.
 * It also includes the facilities for reading the encoder, based off the
 * button's facilities.
 * Written by Koppany Horvath. MIT License. 2023.
 */

#include "button.h"
#include "encoder.h"

ButtonConfig button_config = {
  /* sample_delay_us = */ 1000,
  /* pressed_count   = */ 10,
  /* released_count  = */ 10,
  /* held_delay_ms   = */ 500,
  /* double_click_ms = */ 250,
};

Button button(&button_config, 2);

void setup() {
  Serial.begin(115200);
  delay(3000);  // Give serial monitor time to start displaying
  Serial.println("macrOS v0.0_buttons_dev");
}

StateChangeDetector button_state(button.Pressed());
StateChangeDetector held_state(button.HeldDown());
StateChangeDetector single_state(button.SingleClicked());
StateChangeDetector double_state(button.DoubleClicked());
int counter = 0;

void loop() {
  unsigned long time_start = micros();
  button.Scan();
  unsigned long time_end = micros();
  if (button_state.Changed(button.Pressed()) ||
      held_state.ChangedTo(button.HeldDown(), true) ||
      single_state.ChangedTo(button.SingleClicked(), true) ||
      double_state.ChangedTo(button.DoubleClicked(), true)) {
    counter++;
    Serial.print(counter);
    Serial.print(": (");
    Serial.print(time_end - time_start);
    Serial.println(")");
    if (button_state.PeekChanged()) {
      Serial.print("  Pressed       ");
      Serial.println(button_state.State());
    }
    if (held_state.PeekChanged()) {
      Serial.print("  HeldDown      ");
      Serial.println(held_state.State());
    }
    if (single_state.PeekChanged()) {
      Serial.print("  SingleClicked ");
      Serial.println(single_state.State());
    }
    if (double_state.PeekChanged()) {
      Serial.print("  DoubleClicked ");
      Serial.println(double_state.State());
    }
    Serial.println();
  }
}
