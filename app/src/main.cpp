#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
	const struct device *dev = DEVICE_DT_GET_ANY(bruno_led_sensor);

	if (!device_is_ready(dev)) {
		LOG_ERR("LED sensor not ready");
		return 0;
	}

	LOG_INF("LED sensor ready, use the 'sensor' shell command");
	return 0;
}
