/*
 * SPDX-FileCopyrightText: 2021 Otto-von-Guericke-Universität Magdeburg
 * SPDX-License-Identifier: LGPL-2.1-only
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Blinky application
 *
 * @author      Marian Buschsieweke <marian.buschsieweke@ovgu.de>
 *
 * @}
 */

#include <stdio.h>

#include "clk.h"
#include "board.h"
#include "periph/gpio.h"
#include "periph_conf.h"
#include "timex.h"
#include "ztimer.h"

//#define WS281X_BYTES_PER_DEVICE (4U)

#include "ws281x.h"

gpio_t led0 = GPIO_PIN(3,3);
gpio_mode_t led0_mode = GPIO_OUT;

gpio_t ws_out = GPIO_PIN(3,4);
gpio_mode_t ws_out_mode = GPIO_OUT;

gpio_t button = GPIO_PIN(3,2);

struct config_t {
   bool low;
   config_t() : low(true) {}
   void toggle() { low = !low; }
   unsigned int delay_ms() const { return low ? 200 : 2000; }
};

static void delay(config_t const& c)
{
    if (IS_USED(MODULE_ZTIMER)) {
        ztimer_sleep(ZTIMER_USEC, c.delay_ms()*MS_PER_SEC);
    }
    else {
        /*
         * As fallback for freshly ported boards with no timer drivers written
         * yet, we just use the CPU to delay execution and assume that roughly
         * 20 CPU cycles are spend per loop iteration.
         *
         * Note that the volatile qualifier disables compiler optimizations for
         * all accesses to the counter variable. Without volatile, modern
         * compilers would detect that the loop is only wasting CPU cycles and
         * optimize it out - but here the wasting of CPU cycles is desired.
         */
        uint32_t loops = coreclk() / 20;
        for (volatile uint32_t i = 0; i < loops; i++) { }
    }
}

void button_callback(void* vcfg) {
    config_t& cfg = *reinterpret_cast<config_t*>(vcfg);
    cfg.toggle();
}

#define NUM_LEDS  1

int main(void)
{
    /* Initialize the LED0 pin */
    gpio_init(led0, led0_mode);
    /* Turn off the LED0 pin */
    gpio_clear(led0);

    gpio_init(ws_out, ws_out_mode);
    gpio_clear(ws_out);

    config_t cfg;

    uint8_t led_buf[NUM_LEDS*WS281X_BYTES_PER_DEVICE];
    ws281x_params_t params = { .buf = led_buf, .numof=NUM_LEDS, .pin=ws_out };
    ws281x_t ws_dev;

    const color_rgb_t red = { .r=0xFF,.g=0,.b=0x0};
    const color_rgb_t green = { .r=0x0,.g=0xFF,.b=0x0};

    const int init_ok = ws281x_init(&ws_dev, &params);

    //const int init_ok = gpio_init_int( button, GPIO_IN_PU, GPIO_FALLING, button_callback, &cfg );

    bool state = false;
    while (1) {
        delay(cfg);
        if (state) {
            ws281x_set_buffer(&led_buf,0,red);
        } else {
            ws281x_set_buffer(&led_buf,0,green);
        }
        ws281x_prepare_transmission(&ws_dev);
        ws281x_write(&ws_dev);
        ws281x_end_transmission(&ws_dev);
        state = !state;
        if (init_ok==0)
            gpio_toggle(led0);
        puts("Blink! (No LED present or configured...)");
    }

    return 0;
}
