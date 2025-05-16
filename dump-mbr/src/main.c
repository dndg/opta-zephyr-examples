/*
 * Copyright (c) 2025 DNDG srl
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/flash.h>
#include <board.h>

static const struct device *const flash_dev =
    DEVICE_DT_GET(DT_NODELABEL(external_flash));

struct __attribute__((packed)) MBREntry {
    uint8_t status;
    uint8_t chs_start[3];
    uint8_t type;
    uint8_t chs_end[3];
    uint32_t lba_offset;
    uint32_t lba_size;
};

struct __attribute__((packed)) MBRTable {
    struct MBREntry entries[4];
    uint8_t signature[2];
};

static uint8_t data[512];

int main(void)
{
	const struct MBRTable *table = (struct MBRTable *)&data[0x1BE];

    int ret;

    if (!device_is_ready(flash_dev)) {
        printk("Error: flash device is not ready\n");
		return -ENODEV;
	}

	/* Read and print MBR */
	ret = flash_read(flash_dev, 0, data, 512);
	if (ret != 0) {
	    printk("Error: flash read returned an error: %d\n", ret);
	    return -EIO;
	}

	if (table->signature[0] != 0x55 || table->signature[1] != 0xAA) {
	    printk("Error: MBR signature not found\n");
		return -EIO;
	}

	for (int i = 0; i < 4; i++) {
        printk("\n== Partition %d\n", i + 1);
        if (table->entries[i].type == 0) {
            printk("Not present\n");
            continue;
        }
        printk("Type: 0x%x\n", table->entries[i].type);
        printk("Bootable: %s\n", table->entries[i].status == 0 ? "NO" : "YES");
        printk("LBA offset: 0x%08x\n", table->entries[i].lba_offset * 4096);
        printk("LBA size:   0x%08x\n", table->entries[i].lba_size * 4096);
	}

	return 0;
}

/* Note: this is what the partition table should look on a default Opta:
 *
 * 000001B0: ff ff ff ff ff ff ff ff  ff ff ff ff ff ff 00 00 |........ ........|
 * 000001C0: 02 00 0b 04 00 00 01 00  00 00 ff 00 00 00 00 04 |........ ........|
 * 000001D0: 01 00 0b 38 00 00 00 01  00 00 00 0d 00 00 00 38 |...8.... .......8|
 * 000001E0: 01 00 0b 38 00 00 00 0e  00 00 00 00 00 00 00 00 |...8.... ........|
 * 000001F0: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 55 aa |........ ......U.|
 */
