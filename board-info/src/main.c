/*
 * Copyright (c) 2025 DNDG srl
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <sys/errno.h>
#include <board.h>

static void uint8tohex(char *dst, uint8_t value)
{
    int v;

	for (int i = 0; i < 2; i++) {
		v = (value >> ((2 - i - 1) * 4)) & 0x0F;
		dst[i] = v <= 9 ? (0x30 + v) : (0x40 + v - 9);
	}
}

static void fmthwaddr(char *dst, const uint8_t hwaddr[6])
{
    uint8tohex(&dst[0], hwaddr[0]);
	uint8tohex(&dst[3], hwaddr[1]);
	uint8tohex(&dst[6], hwaddr[2]);
	uint8tohex(&dst[9], hwaddr[3]);
	uint8tohex(&dst[12], hwaddr[4]);
	uint8tohex(&dst[15], hwaddr[5]);
	dst[2] = dst[5] = dst[8] = dst[11] = dst[14] = ':';
	dst[17] = '\0';
}


int main(void)
{
    char hwaddr_str1[17];
    char hwaddr_str2[17];

	const struct opta_board_info *info = opta_get_board_info();

	if (info == NULL) {
	    printk("Error: could not get board info (hint: enable CONFIG_FLASH)\n");
		return -ENODEV;
	}

	fmthwaddr(hwaddr_str1, info->mac_address);
	fmthwaddr(hwaddr_str2, info->mac_address_wifi);

    printk("Opta %s\n", opta_get_serial_number());

    printk("Version:  %d\n", info->version);

    printk("Revision: %d\n", info->revision);

    printk("USB VID:  0x%04x\n", info->vid);

    printk("USB PID:  0x%04x\n", info->pid);

    printk("RS485:    %s\n",
        info->_board_functionalities_bits.rs485 ? "Present" : "Not present");

    printk("Ethernet: %s, MAC: %s\n",
        info->_board_functionalities_bits.ethernet ? "Present" : "Not present (error?)",
        hwaddr_str1);

    if (info->_board_functionalities_bits.wifi) {
        printk("WiFi/BLE: Present, MAC: %s\n", hwaddr_str2);

    }
    else {
        printk("WiFi/BLE: Not present\n");
    }

    return 0;
}
