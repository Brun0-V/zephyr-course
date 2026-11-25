#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT bruno_led_sensor
LOG_MODULE_REGISTER(led_sensor, CONFIG_SENSOR_LOG_LEVEL);

struct led_sensor_config {
	struct gpio_dt_spec led;
};

struct led_sensor_data {
	uint32_t on_count;
};

/* Turns the LED on */
static int led_sensor_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
	const struct led_sensor_config *cfg = dev->config;
	struct led_sensor_data *data = dev->data;

	data->on_count++;
	return gpio_pin_set_dt(&cfg->led, 1);
}

/* Turns the LED off and reports how many times it was turned on */
static int led_sensor_channel_get(const struct device *dev, enum sensor_channel chan,
				  struct sensor_value *val)
{
	const struct led_sensor_config *cfg = dev->config;
	struct led_sensor_data *data = dev->data;

	val->val1 = data->on_count;
	val->val2 = 0;
	return gpio_pin_set_dt(&cfg->led, 0);
}

static DEVICE_API(sensor, led_sensor_api) = {
	.sample_fetch = led_sensor_sample_fetch,
	.channel_get = led_sensor_channel_get,
};

static int led_sensor_init(const struct device *dev)
{
	const struct led_sensor_config *cfg = dev->config;

	if (!gpio_is_ready_dt(&cfg->led)) {
		return -ENODEV;
	}

	return gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE);
}

#define LED_SENSOR_DEFINE(inst)                                                   \
	static struct led_sensor_data data_##inst;                                \
	static const struct led_sensor_config cfg_##inst = {                      \
		.led = GPIO_DT_SPEC_INST_GET(inst, gpios),                         \
	};                                                                        \
	DEVICE_DT_INST_DEFINE(inst, led_sensor_init, NULL, &data_##inst,          \
			      &cfg_##inst, POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY, \
			      &led_sensor_api);

DT_INST_FOREACH_STATUS_OKAY(LED_SENSOR_DEFINE)
