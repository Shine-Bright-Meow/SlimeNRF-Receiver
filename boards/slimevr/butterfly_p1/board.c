#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/init.h>

static int board_led_fix_init(const struct device *dev)
{
	ARG_UNUSED(dev);

	const struct device *gpio0 = DEVICE_DT_GET(DT_NODELABEL(gpio0));

	if (!device_is_ready(gpio0)) {
		return -ENODEV;
	}

	/* Force LED pin high (open-drain) to turn it off */
	gpio_pin_configure(gpio0, 3, GPIO_OUTPUT_HIGH | GPIO_OPEN_DRAIN);

	return 0;
}

/* APPLICATION level so it runs after LED/PWM drivers */
SYS_INIT(board_led_fix_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
