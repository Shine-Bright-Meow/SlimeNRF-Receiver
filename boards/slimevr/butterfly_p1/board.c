/*
 * Copyright (c) 2018 Nordic Semiconductor ASA.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>
#include <hal/nrf_gpio.h>

LOG_MODULE_REGISTER(board, LOG_LEVEL_INF);

static struct k_work_delayable led_fix_work;

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

static const struct pwm_dt_spec pwm_led = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));

static void led_fix_work_handler(struct k_work *work)
{
	int ret;
	
	LOG_INF("Fixing LED state...");
	
	if (device_is_ready(led.port)) {
		gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
		gpio_pin_set_dt(&led, 0); // 0 = inactive = HIGH for active-low
	}
	
	if (device_is_ready(pwm_led.dev)) {
		ret = pwm_set_dt(&pwm_led, PWM_MSEC(20), PWM_MSEC(20));
		if (ret == 0) {
			LOG_INF("LED state fixed: OFF");
		} else {
			LOG_ERR("Failed to set PWM: %d", ret);
		}
	}
}

static int board_led_init(void)
{
	nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, 3));
	nrf_gpio_pin_set(NRF_GPIO_PIN_MAP(0, 3));
	
	k_work_init_delayable(&led_fix_work, led_fix_work_handler);
	
	k_work_schedule(&led_fix_work, K_MSEC(100));
	
	return 0;
}

SYS_INIT(board_led_init, POST_KERNEL, 0);