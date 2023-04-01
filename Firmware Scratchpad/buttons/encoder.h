#ifndef ENCODER_H
#define ENCODER_H

class Encoder {
  uint8_t clock_pin_;
  uint8_t data_pin_;
  uint8_t button_pin_;
  Encoder(uint8_t clock_pin, uint8_t data_pin, uint8_t button_pin) {
    clock_pin_ = clock_pin;
    data_pin_ = data_pin;
    button_pin_ = button_pin;
  }
};

#endif
