#ifndef BUTTON_H
#define BUTTON_H

struct ButtonConfig {
  uint16_t sample_delay_us;  // After how many us should we check the button
  uint8_t pressed_count;     // Count needed to flip to pressed state
  uint8_t released_count;    // Count needed to flip to unpressed state
  uint16_t held_delay_ms;    // After how many ms should a press be a hold
  uint16_t double_click_ms;  // Time to wait for a press after a press
};

// Provides debounce, single/double click detection, and hold detection.
class Button {
 public:
  Button(ButtonConfig* button_config, uint8_t button_pin) {
    button_config_ = button_config;
    button_pin_ = button_pin;

    pinMode(button_pin_, INPUT_PULLUP);
    
    last_scan_us_ = micros();
    pressed_count_ = 0;
    released_count_ = 0;
    
    pressed_ = false;
    prev_pressed_state_change_ms_ = millis();
    pressed_state_change_ms_ = prev_pressed_state_change_ms_;
  }

  // The debouncing routine.
  void Scan() {
    unsigned long now_time_us = micros();
    unsigned long delta_us_ = now_time_us - last_scan_us_;
    if (delta_us_ < button_config_->sample_delay_us)
      return;
    last_scan_us_ = now_time_us;
    if (!digitalRead(button_pin_)) {  // Pullup makes it inverted
      released_count_ = 0;
      if (pressed_count_ != 255)
        pressed_count_++;
      if (!pressed_ && pressed_count_ >= button_config_->pressed_count) {
        pressed_ = true;
        prev_pressed_state_change_ms_ = pressed_state_change_ms_;
        pressed_state_change_ms_ = millis();
      }
    } else {
      pressed_count_ = 0;
      if (released_count_ != 255)
        released_count_++;
      if (pressed_ && released_count_ >= button_config_->released_count) {
        pressed_ = false;
        prev_pressed_state_change_ms_ = pressed_state_change_ms_;
        pressed_state_change_ms_ = millis();
      }
    }
  }

  bool Pressed() { return pressed_; }
  
  bool HeldDown() {
    unsigned long delta_ms = millis() - pressed_state_change_ms_;
    return pressed_ && delta_ms >= button_config_->held_delay_ms;
  }

  // SingleClicked and DoubleClicked need to be monitored for state transitions
  bool SingleClicked() {
    unsigned long delta_ms = millis() - pressed_state_change_ms_;
    return !pressed_ && delta_ms > button_config_->double_click_ms;
  }
  bool DoubleClicked() {
    unsigned long delta_ms =
      pressed_state_change_ms_ - prev_pressed_state_change_ms_;
    return pressed_ && delta_ms <= button_config_->double_click_ms;
  }

 private:
  ButtonConfig* button_config_;
  uint8_t button_pin_;

  unsigned long last_scan_us_;
  uint8_t pressed_count_;
  uint8_t released_count_;
  
  bool pressed_;  // The official "instantaneous" state of the button
  unsigned long pressed_state_change_ms_;  // When the state last changed
  unsigned long prev_pressed_state_change_ms_;  // When the state last changed
};

// Convenience object to let us know if a value has changed.
class StateChangeDetector {
 public:
  StateChangeDetector(bool initial = false) {
    latest_state_ = initial;
    prev_state_ = initial;
  }
  bool Changed(bool new_state) {
    prev_state_ = latest_state_;
    latest_state_ = new_state;
    return latest_state_ != prev_state_;
  }
  bool ChangedTo(bool new_state, bool expected) {
    return Changed(new_state) && new_state == expected;
  }
  bool PeekChanged() {
    return latest_state_ != prev_state_;
  }
  bool State() { return latest_state_; }
 private:
  bool latest_state_;
  bool prev_state_;
};

#endif
