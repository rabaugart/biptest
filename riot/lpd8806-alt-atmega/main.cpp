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

#include "etl/array.h"

#include "clk.h"
#include "board.h"
#include "periph/gpio.h"
#include "periph_conf.h"
#include "timex.h"
#include "ztimer.h"
#include "ztimer/stopwatch.h"

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
   ztimer_stopwatch_t stopwatch;
   config_t() : low(true) {
       ztimer_stopwatch_init(ZTIMER_MSEC,&stopwatch);
   }
   void toggle() { low = !low; }
   unsigned int delay_ms() const { return low ? 2000 : 200; }
};

static void delay(config_t const& c)
{
    ztimer_sleep(ZTIMER_MSEC, c.delay_ms());
}

void button_callback(void* vcfg) {
    config_t& cfg = *reinterpret_cast<config_t*>(vcfg);
    if (!gpio_read(button)) {
        // Button low ist gedrückt
        ztimer_stopwatch_start(&cfg.stopwatch);
    } else {
        if (ztimer_stopwatch_measure(&cfg.stopwatch)>100) {
            cfg.toggle();
        }
        ztimer_stopwatch_stop(&cfg.stopwatch);
        ztimer_stopwatch_reset(&cfg.stopwatch);
    }
}

static constexpr size_t NUM_LEDS = 3;

using led_array_t = etl::array<color_rgb_t,NUM_LEDS>;

static constexpr color_rgb_t red = { .r=0xFF,.g=0,.b=0x00};
static constexpr color_rgb_t blue = { .r=0x00,.g=0xFF,.b=0x00};
static constexpr color_rgb_t green = { .r=0x00,.g=0x00,.b=0xFF};
static constexpr color_rgb_t gelb = { .r=0xFF,.g=0xFF,.b=0x00};
static constexpr color_rgb_t weiss = { .r=0xFF,.g=0xFF,.b=0xFF};

static constexpr etl::array SEQ = {
    led_array_t{red,green,blue},
    led_array_t{blue,red,green},
    led_array_t{green,blue,red},
    led_array_t{gelb,gelb,gelb},
    led_array_t{red,red,red},
    led_array_t{green,green,green},
    led_array_t{weiss,weiss,weiss}
};

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

    const int init_ok = lpd8806_init(&lpd8806_dev, &params);

    const int init_okb = gpio_init_int( button, GPIO_IN_PU, GPIO_BOTH, button_callback, &cfg );

    size_t state = 0;
    while (1) {
        delay(cfg);
        lpd8806_load_rgb(&lpd8806_dev,const_cast<color_rgb_t*>(&(SEQ[state][0])));
        state = (state+1) % SEQ.size();
        if (init_ok==0 && init_okb==0)
            gpio_toggle(led0);
        puts("Blink! (No LED present or configured...)");
    }

    return 0;
}
