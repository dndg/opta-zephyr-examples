/*
 * Copyright (c) 2025 DNDG srl
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "cryptoauthlib/atca_basic.h"
#include "cryptoauthlib/atca_status.h"
#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <board.h>
#include <cryptoauthlib/cryptoauthlib.h>

int main(void)
{
    ATCA_STATUS status;

    cfg_ateccx08a_i2c_default.atcai2c.bus = 1;
    cfg_ateccx08a_i2c_default.atcai2c.address = 0xC0;
    cfg_ateccx08a_i2c_default.cfg_data = DEVICE_DT_NAME(DT_NODELABEL(i2c1));

    status = atcab_init(&cfg_ateccx08a_i2c_default);
    if (status != ATCA_SUCCESS) {
        printk("Error: atcab_init() returned error status: %d\n", status);
    }

	return 0;
}
