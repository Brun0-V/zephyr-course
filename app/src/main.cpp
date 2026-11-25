#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
	const struct device *dev = DEVICE_DT_GET_ANY(bruno_led_sensor);
	struct sensor_value val;

	if (!device_is_ready(dev)) {
		LOG_ERR("LED sensor not ready");
		return 0;
	}

	while (1) {
		sensor_sample_fetch(dev); /* LED on */
		k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);

		sensor_channel_get(dev, SENSOR_CHAN_ALL, &val); /* LED off */
		LOG_INF("LED turned on %d times", val.val1);
		k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
	}

	return 0;
}
