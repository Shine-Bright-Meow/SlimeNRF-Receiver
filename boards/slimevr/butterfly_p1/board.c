/*
 * Copyright (c) 2018 Nordic Semiconductor ASA.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/init.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>
#include <hal/nrf_gpio.h>

LOG_MODULE_REGISTER(board, LOG_LEVEL_INF);

static int board_early_init(void)
{
	nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, 3));
	nrf_gpio_pin_set(NRF_GPIO_PIN_MAP(0, 3));
	
	return 0;
}

SYS_INIT(board_early_init, PRE_KERNEL_1, 0);

static int board_pwm_init(void)
{
	const struct device *pwm_dev;
	int ret;

	pwm_dev = DEVICE_DT_GET(DT_NODELABEL(pwm0));
	if (!device_is_ready(pwm_dev)) {
		LOG_WRN("PWM device not ready yet");
		return -ENODEV;
	}

	ret = pwm_set(pwm_dev, 0, PWM_MSEC(20), PWM_MSEC(20));
	if (ret) {
		LOG_ERR("Failed to set PWM to OFF state: %d", ret);
		return ret;
	}

	LOG_INF("PWM initialized with LED in OFF state");
	
	return 0;
}

SYS_INIT(board_pwm_init, POST_KERNEL, 99);