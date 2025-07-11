/*
 * Copyright (c) 2025 DNDG srl
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/util.h>
#include <cryptoauthlib/cryptoauthlib.h>
#include <board.h>

static const struct device *i2c1 = DEVICE_DT_GET(DT_NODELABEL(i2c1));

static uint8_t msg[ATCA_SHA256_DIGEST_SIZE];
static uint8_t publickey[ATCA_ECCP256_PUBKEY_SIZE];
static uint8_t signature[ATCA_ECCP256_SIG_SIZE];
static uint8_t publickey_hex[ATCA_ECCP256_PUBKEY_SIZE * 2 + 1];
static char signature_hex[ATCA_ECCP256_SIG_SIZE * 2 + 1];

int main(void)
{
    ATCA_STATUS status;

    if (!device_is_ready(i2c1)) {
		printk("Error: I2C1 device not ready\n");
		return 0;
	}

    cfg_ateccx08a_i2c_default.atcai2c.bus = 0;
    cfg_ateccx08a_i2c_default.atcai2c.address = 0xC0;
    cfg_ateccx08a_i2c_default.cfg_data = DEVICE_DT_NAME(DT_NODELABEL(i2c1));

    status = atcab_init(&cfg_ateccx08a_i2c_default);
    if (status != ATCA_SUCCESS) {
        printk("Error: atcab_init() returned error status: %d\n", status);
        return -1;
    }

    status = atcac_sw_sha2_256((uint8_t*)"The quick brown fox jumps over the lazy dog.", 44, msg);
    if (status != ATCA_SUCCESS) {
        printk("Error: atcac_sw_sha2_256() returned error status: %d\n", status);
        return -2;
    }

    status = atcab_sign(0, msg, signature);
    if (status != ATCA_SUCCESS) {
        printk("Error: atcab_sign() returned error status: %d\n", status);
        return -3;
    }

    status = atcab_get_pubkey(0, publickey);
    if (status != ATCA_SUCCESS) {
        printk("Error: atcab_get_pubkey() returned error status: %d\n", status);
        return -4;
    }

    bin2hex(signature, sizeof(signature), signature_hex, sizeof(signature_hex));
    printk("Signature: %s\n", signature_hex);
    bin2hex(publickey, sizeof(publickey), publickey_hex, sizeof(publickey_hex));
    printk("PublicKey: %s\n", publickey_hex);

	return 0;
}
