/*
 * Copyright (c) 2018 Nordic Semiconductor ASA.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/drivers/gpio.h>

static int early_led_off(void)
{
    const struct device *gpio0 = DEVICE_DT_GET(DT_NODELABEL(gpio0));

    if (!device_is_ready(gpio0)) {
        return -ENODEV;
    }

    gpio_pin_configure(gpio0, 3, GPIO_OUTPUT_HIGH | GPIO_OPEN_DRAIN);

    return 0;
}
SYS_INIT(early_led_off, PRE_KERNEL_1, 0);