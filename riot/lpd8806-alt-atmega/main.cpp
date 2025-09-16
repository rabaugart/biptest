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

#include "lpd8806.h"

gpio_t led0 = GPIO_PIN(3,3);
gpio_mode_t led0_mode = GPIO_OUT;

gpio_t data_out = GPIO_PIN(3,4);
gpio_mode_t data_out_mode = GPIO_OUT;

gpio_t clk_out = GPIO_PIN(3,5);
gpio_mode_t clk_out_mode = GPIO_OUT;

gpio_t button = GPIO_PIN(3,2);

struct config_t {
   bool low;
   config_t() : low(true) {}
   void toggle() { low = !low; }
   unsigned int delay_ms() const { return low ? 2000 : 2000; }
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

    gpio_init(data_out, data_out_mode);
    gpio_clear(data_out);
    gpio_init(clk_out, clk_out_mode);
    gpio_clear(clk_out);

    config_t cfg;

    lpd8806_params_t params = { .led_cnt=NUM_LEDS, .pin_clk=clk_out, .pin_dat=data_out };
    lpd8806_t lpd8806_dev;

    color_rgb_t red = { .r=0xFF,.g=0,.b=0x00};
    color_rgb_t green = { .r=0x00,.g=0x00,.b=0xFF};

    const int init_ok = lpd8806_init(&lpd8806_dev, &params);

    //const int init_ok = gpio_init_int( button, GPIO_IN_PU, GPIO_FALLING, button_callback, &cfg );

    bool state = false;
    while (1) {
        delay(cfg);
        if (state) {
            lpd8806_load_rgb(&lpd8806_dev,&red);
        } else {
            lpd8806_load_rgb(&lpd8806_dev,&green);
        }
        state = !state;
        if (init_ok==0)
            gpio_toggle(led0);
        puts("Blink! (No LED present or configured...)");
    }

    return 0;
}
