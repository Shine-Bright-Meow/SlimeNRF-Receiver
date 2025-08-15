/*
 * Copyright (c) 2018 Nordic Semiconductor ASA.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <hal/nrf_gpio.h>

static struct k_work_delayable led_fix_work;
static const struct pwm_dt_spec pwm_led = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));

static void led_fix_work_handler(struct k_work *work)
{
	if (device_is_ready(pwm_led.dev)) {
		/* For active-LOW: 100% duty = HIGH = LED OFF */
		pwm_set_dt(&pwm_led, PWM_MSEC(1), PWM_MSEC(1));
	}
}

static int board_led_init(void)
{
	/* Set pin HIGH immediately (LED OFF for active-LOW) */
	nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, 3));
	nrf_gpio_pin_set(NRF_GPIO_PIN_MAP(0, 3));  // Set = HIGH = OFF
	
	k_work_init_delayable(&led_fix_work, led_fix_work_handler);
	k_work_schedule(&led_fix_work, K_MSEC(100));
	
	return 0;
}

SYS_INIT(board_led_init, PRE_KERNEL_1, 0);