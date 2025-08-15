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
#include <hal/nrf_pwm.h>

LOG_MODULE_REGISTER(board, LOG_LEVEL_INF);

#define PWM_NODE DT_NODELABEL(pwm0)
#define LED_PIN 3

static int board_pre_init(void)
{
	nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(0, LED_PIN));
	nrf_gpio_pin_set(NRF_GPIO_PIN_MAP(0, LED_PIN));
	
	return 0;
}
SYS_INIT(board_pre_init, PRE_KERNEL_1, 0);

static void fix_pwm_led_state(void)
{
	const struct device *pwm_dev;
	NRF_PWM_Type *pwm_reg = (NRF_PWM_Type *)DT_REG_ADDR(PWM_NODE);
	
	pwm_dev = DEVICE_DT_GET(PWM_NODE);
	if (!device_is_ready(pwm_dev)) {
		return;
	}

	nrf_pwm_task_trigger(pwm_reg, NRF_PWM_TASK_STOP);
	
	nrf_pwm_configure(pwm_reg, NRF_PWM_CLK_1MHz, NRF_PWM_MODE_UP, 20000);
	
	nrf_pwm_seq_cnt_set(pwm_reg, 0, 20000);
	
	pwm_set(pwm_dev, 0, PWM_MSEC(20), PWM_MSEC(20));
	
	LOG_INF("PWM LED state corrected to OFF");
}

static int board_device_init(void)
{
	k_msleep(10);
	fix_pwm_led_state();
	
	return 0;
}

SYS_INIT(board_device_init, APPLICATION, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);