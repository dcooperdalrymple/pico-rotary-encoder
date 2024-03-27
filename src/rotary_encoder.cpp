#include "rotary_encoder.hpp"

uint8_t RotaryEncoder::num_encoders = 0;

PIO RotaryEncoder::pio[MAX_ENCODERS] = {NULL, NULL};

int RotaryEncoder::pin_clk[MAX_ENCODERS] = {-1, -1};
int RotaryEncoder::pin_dt[MAX_ENCODERS] = {-1, -1};
int RotaryEncoder::pin_sw[MAX_ENCODERS] = {-1, -1};

uint8_t RotaryEncoder::sw_state[MAX_ENCODERS] = {0, 0};

rotary_encoder_callback_t RotaryEncoder::cb[MAX_ENCODERS] = {NULL, NULL};

RotaryEncoder::RotaryEncoder(int clk, int sw) {
    if (num_encoders >= MAX_ENCODERS) return;
    
    index = num_encoders++;
    sm = 0;

    pin_clk[index] = clk;
    pin_dt[index] = clk+1;
    pin_sw[index] = sw;

    // Configure pins and PIO program
    if (index == 0) {
        pio[index] = pio0;
    } else {
        pio[index] = pio1;
    }

    pio_gpio_init(pio[index], pin_clk[index]);
    pio_gpio_init(pio[index], pin_dt[index]);

    uint offset = pio_add_program(pio[index], &pio_rotary_encoder_program);
    pio_sm_config c = pio_rotary_encoder_program_get_default_config(offset);

    sm_config_set_in_pins(&c, pin_clk[index]);
    sm_config_set_in_shift(&c, false, false, 0); // LSB, no autopush

    // Configure IRQ
    if (index == 0) {
        irq_set_exclusive_handler(PIO0_IRQ_0, &process_rotary);
        irq_set_enabled(PIO0_IRQ_0, true);
        pio0_hw->inte0 = PIO_IRQ0_INTE_SM0_BITS | PIO_IRQ0_INTE_SM1_BITS;
    } else {
        irq_set_exclusive_handler(PIO1_IRQ_0, &process_rotary);
        irq_set_enabled(PIO1_IRQ_0, true);
        pio1_hw->inte0 = PIO_IRQ0_INTE_SM0_BITS | PIO_IRQ0_INTE_SM1_BITS;
    }

    // Setup the State Machine
    pio_sm_init(pio[index], sm, 16, &c); // 16 = program counter after jump table
    pio_sm_set_enabled(pio[index], sm, true);

    if (pin_sw[index] >= 0) {
        gpio_init(pin_sw[index]);
        gpio_set_dir(pin_sw[index], GPIO_IN);
        gpio_set_irq_enabled_with_callback(pin_sw[index], GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &process_switch);
    }
};

RotaryEncoder::RotaryEncoder(int clk) {
    RotaryEncoder(clk, -1);
};

void RotaryEncoder::set_callback(rotary_encoder_callback_t callback) {
    cb[index] = callback;
};

void RotaryEncoder::clear_callback() {
    cb[index] = NULL;
};

void RotaryEncoder::process_rotary() {
    io_rw_32 *irq;
    int i;
    if (pio0_hw->irq & 3) {
        irq = &pio0_hw->irq;
        i = 0;
    } else if (pio1_hw->irq & 3) {
        irq = &pio1_hw->irq;
        i = 1;
    } else {
        return;
    }

    if (*irq & 1) {
        if (cb[i]) cb[i](RotaryEncoderEvent::CW);
    } else if (*irq & 2) {
        if (cb[i]) cb[i](RotaryEncoderEvent::CCW);
    }

    // Clear interrupts
    *irq = 3;
};

void RotaryEncoder::process_switch(uint gpio, uint32_t events) {
    gpio_acknowledge_irq(gpio, events);

    int i;
    for (i = 0; i < num_encoders; i++) {
        if (gpio == pin_sw[i]) break;
        if (i == num_encoders-1) {
            i = -1;
            break;
        }
    }
    if (i < 0) return;

    if (sw_state[i] == (uint8_t)gpio_get(gpio)) return;

    sw_state[i] = (uint8_t)gpio_get(gpio);
    if (sw_state[i]) {
        if (cb[i]) cb[i](RotaryEncoderEvent::RELEASE);
    } else {
        if (cb[i]) cb[i](RotaryEncoderEvent::PRESS);
    }
};
