/*
 * Copyright (C) 2016 Hauke Petersen <devel@haukepetersen.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#pragma once

/**
 * @ingroup     drivers_lpd8806
 *
 * @{
 * @file
 * @brief       Default configuration for lpd8806 based LED strips
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 */

#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    Set default configuration parameters for the lpd8806 driver
 * @{
 */
#ifndef LPD8806_PARAM_LED_CNT
#define LPD8806_PARAM_LED_CNT       (64)    /* most have 64 per meter... */
#endif
#ifndef LPD8806_PARAM_PIN_CLK
#define LPD8806_PARAM_PIN_CLK       (GPIO_PIN(0, 0))
#endif
#ifndef LPD8806_PARAM_PIN_DAT
#define LPD8806_PARAM_PIN_DAT       (GPIO_PIN(0, 1))
#endif

#ifndef LPD8806_PARAMS
#define LPD8806_PARAMS              { .led_cnt = LPD8806_PARAM_LED_CNT, \
                                      .pin_clk = LPD8806_PARAM_PIN_CLK, \
                                      .pin_dat = LPD8806_PARAM_PIN_DAT }
#endif
/**@}*/

/**
 * @brief   LPD8806 parameter allocation
 */
static const lpd8806_params_t lpd8806_params[] =
{
    LPD8806_PARAMS
};

#ifdef __cplusplus
}
#endif

/** @} */
