#include "globals.h"
#include "system.h"

#define USB DT_NODELABEL(usbd)
#if DT_NODE_HAS_STATUS(USB, okay)

#include <zephyr/console/console.h>
#include <zephyr/sys/reboot.h>

#include <ctype.h>

LOG_MODULE_REGISTER(console, LOG_LEVEL_INF);

static void console_thread(void);
K_THREAD_DEFINE(console_thread_id, 512, console_thread, NULL, NULL, NULL, 6, 0, 0);

#define DFU_EXISTS CONFIG_BUILD_OUTPUT_UF2

static void console_thread(void)
{
	console_getline_init();
	printk("*** " CONFIG_USB_DEVICE_MANUFACTURER " " CONFIG_USB_DEVICE_PRODUCT " ***\n");
	printk("reboot                       Soft reset the device\n");
	printk("pair                         Enter pairing mode\n");
	printk("clear                        Clear stored devices\n");

	uint8_t command_reboot[] = "reboot";
	uint8_t command_pair[] = "pair";
	uint8_t command_clear[] = "clear";

#if DFU_EXISTS
	printk("dfu                          Enter DFU bootloader\n");

	uint8_t command_dfu[] = "dfu";
#endif

	while (1) {
		uint8_t *line = console_getline();
		for (uint8_t *p = line; *p; ++p) {
			*p = tolower(*p);
		}

		if (memcmp(line, command_reboot, sizeof(command_reboot)) == 0)
		{
			sys_reboot(SYS_REBOOT_COLD);
		}
		else if (memcmp(line, command_pair, sizeof(command_pair)) == 0)
		{
			reboot_counter_write(101);
			k_msleep(1);
			sys_reboot(SYS_REBOOT_WARM);
		}
		else if (memcmp(line, command_clear, sizeof(command_clear)) == 0) 
		{
			reboot_counter_write(102);
			k_msleep(1);
			sys_reboot(SYS_REBOOT_WARM);
		}
#if DFU_EXISTS
		else if (memcmp(line, command_dfu, sizeof(command_dfu)) == 0)
		{
			NRF_POWER->GPREGRET = 0x57;
			sys_reboot(SYS_REBOOT_COLD);
		}
#endif
		else
		{
			printk("Unknown command\n");
		}
	}
}

#endif