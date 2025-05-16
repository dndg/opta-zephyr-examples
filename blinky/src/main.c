/*
 * Copyright (c) 2025 DNDG srl
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(blinky, LOG_LEVEL_DBG);

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(DT_ALIAS(led3), gpios);

int main(void) {
  int ret, led = 0;

  if (!gpio_is_ready_dt(&led0) || !gpio_is_ready_dt(&led1) ||
      !gpio_is_ready_dt(&led2) || !gpio_is_ready_dt(&led3)) {
    return -ENODEV;
  }

  ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
  if (ret < 0) {
    return -EIO;
  }
  ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
  if (ret < 0) {
    return -EIO;
  }
  ret = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
  if (ret < 0) {
    return -EIO;
  }
  ret = gpio_pin_configure_dt(&led3, GPIO_OUTPUT_ACTIVE);
  if (ret < 0) {
    return -EIO;
  }

  while (1) {
    switch (led) {
    case 0:
      gpio_pin_toggle_dt(&led0);
      break;
    case 1:
      gpio_pin_toggle_dt(&led1);
      break;
    case 2:
      gpio_pin_toggle_dt(&led2);
      break;
    case 3:
      gpio_pin_toggle_dt(&led3);
      break;
    }

    LOG_INF("Switched LED %d state", led);

    led = (led + 1) % 4;

    k_msleep(500);
  }

  return 0;
}
