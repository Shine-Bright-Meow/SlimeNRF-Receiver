/*
 * Copyright (c) 2018 Nordic Semiconductor ASA.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/init.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <hal/nrf_gpio.h>

static int board_early_init(void)
{
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, 3));
    nrf_gpio_pin_set(NRF_GPIO_PIN_MAP(0, 3));  // Set HIGH (LED OFF)
    
    return 0;
}

SYS_INIT(board_early_init, PRE_KERNEL_1, 0);

static int board_gpio_init(void)
{
    const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
    
    if (!device_is_ready(led.port)) {
        return -ENODEV;
    }
    
    gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    gpio_pin_set_dt(&led, 0);  // Ensure it stays OFF
    
    return 0;
}

SYS_INIT(board_gpio_init, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);