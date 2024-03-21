#include "rotary_encoder.hpp"

uint8_t RotaryEncoder::num_encoders = 0;

PIO RotaryEncoder::pio[MAX_ENCODERS] = {NULL, NULL};

int RotaryEncoder::pin_clk[MAX_ENCODERS] = {-1, -1};
int RotaryEncoder::pin_dt[MAX_ENCODERS] = {-1, -1};
int RotaryEncoder::pin_sw[MAX_ENCODERS] = {-1, -1};

uint8_t RotaryEncoder::sw_state[MAX_ENCODERS] = {0, 0};

rotary_encoder_callback_t RotaryEncoder::cb[MAX_ENCODERS] = {NULL, NULL};
