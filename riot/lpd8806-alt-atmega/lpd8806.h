/*
 * Copyright (C) 2016 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#pragma once

/**
 * @defgroup    drivers_lpd8806 lpd8806 based LED Strip
 * @ingroup     drivers_actuators
 * @brief       Driver for lpd8806 based LED strips
 *
 * lpd8806 based LED strips consist of a number of LEDs driven by lpd8806 chips.
 * In these strips, each LED can be controlled individually. For this, every two
 * LEDs are driven by a lpd8806 chip, which are chained and form a long shift
 * register. To control a certain LED, the target color value needs to be
 * shifted to the LEDs position on the strip.
 * This is a modified version of the lpd8808 module of RIOT.
 *
 * This driver implementation does not buffer the current values for each LED.
 * It expects the application to take care of this.
 *
 * @{
 * @file
 * @brief       Interface definition for the lpd8806 LED strip driver
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 * @author      RaBa
 */

#include "color.h"
#include "periph/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Parameters needed for configuration
 */
typedef struct {
    int led_cnt;        /**< number of LEDs on the strip */
    gpio_t pin_clk;     /**< pin connected to the strip's clock signal */
    gpio_t pin_dat;     /**< pin connected to the strip's data signal */
} lpd8806_params_t;

/**
 * @brief   Device descriptor, same as the configuration parameters
 */
typedef lpd8806_params_t lpd8806_t;

/**
 * @brief   Initialize the given lpd8806 based LED strip
 *
 * @param[out] dev      device to initialize
 * @param[in]  params   parameters used for initialization
 *
 * @return  0 on success
 * @return  <0 on error
 */
int lpd8806_init(lpd8806_t *dev, const lpd8806_params_t *params);

/**
 * @brief   Set the color value of each LED on the strip
 *
 * This function sets the color value of each LED on the strip by shifting out
 * the corresponding color values one after the other. The function expects an
 * array of @p color_rgb_t values of the same length as LEDs on the strip.
 *
 * @param[in] dev       device to load color values to
 * @param[in] vals      array of color values, MUST be of same length as LEDs on
 *                      the strip
 */
void lpd8806_load_rgb(const lpd8806_t *dev, color_rgb_t vals[]);

#ifdef __cplusplus
}
#endif

/** @} */
