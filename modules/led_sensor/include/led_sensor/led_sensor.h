#ifndef LED_SENSOR_H_
#define LED_SENSOR_H_

#include <stdbool.h>
#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

/* When disabled, sensor_sample_fetch() no longer turns the LED on */
int led_sensor_set_enabled(const struct device *dev, bool enabled);

#ifdef __cplusplus
}
#endif

#endif /* LED_SENSOR_H_ */
