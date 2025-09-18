/*
 * Copyright (C) 2016 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_lpd8806
 * @{
 *
 * @file
 * @brief       lpd8806 based LED strip driver implementation
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <string.h>

#include "lpd8806.h"

/**
 * @brief   Shift a single byte to the strip
 *
 * @param[in] dev   device to use
 * @param[in] d     byte to shift out
 */
static void put_byte(const lpd8806_t *dev, uint8_t d)
{
    for (int i = 0; i < 8; i++) {
        gpio_write(dev->pin_dat, d & 0x80);
        gpio_set(dev->pin_clk);
        gpio_clear(dev->pin_clk);
        d <<= 1;
    }
}

/**
 * @brief   Flush the previous input
 *
 * lpd8806 based strips need to be flushed after loading values for each LED.
 * This is done by feeding the strip with one zero byte for every 32 LEDs on
 * the strip.
 *
 * @param[in] dev   device to flush
 */
static void flush(const lpd8806_t *dev)
{
    for (int i = 0; i < ((dev->led_cnt + 31) / 32); i++) {
        put_byte(dev, 0);
    }
}

int lpd8806_init(lpd8806_t *dev, const lpd8806_params_t *params)
{
    *dev = *params;

    /* initialize pins */
    gpio_init(dev->pin_dat, GPIO_OUT);
    gpio_init(dev->pin_clk, GPIO_OUT);
    flush(dev);

    return 0;
}

void lpd8806_load_rgb(const lpd8806_t *dev, color_rgb_t vals[])
{
    for (int i = 0; i < dev->led_cnt; i++) {
        put_byte(dev, vals[i].r);
        put_byte(dev, vals[i].b);
        put_byte(dev, vals[i].g);
    }
    flush(dev);
}
