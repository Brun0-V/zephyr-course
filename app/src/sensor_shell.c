#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>

static const struct device *const dev = DEVICE_DT_GET_ANY(bruno_led_sensor);

static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv)
{
	int ret = sensor_sample_fetch(dev);

	if (ret < 0) {
		shell_error(sh, "Fetch failed (%d)", ret);
		return ret;
	}

	shell_print(sh, "LED on");
	return 0;
}

static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv)
{
	struct sensor_value val;
	int ret = sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);

	if (ret < 0) {
		shell_error(sh, "Read failed (%d)", ret);
		return ret;
	}

	shell_print(sh, "LED off, turned on %d times", val.val1);
	return 0;
}

static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv)
{
	if (dev == NULL) {
		shell_error(sh, "No LED sensor in the devicetree");
		return -ENODEV;
	}

	shell_print(sh, "Device: %s", dev->name);
	shell_print(sh, "Ready: %s", device_is_ready(dev) ? "yes" : "no");
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
	SHELL_CMD(fetch, NULL, "Turn the LED on (sensor_sample_fetch)", cmd_sensor_fetch),
	SHELL_CMD(read, NULL, "Turn the LED off and print the count (sensor_channel_get)",
		  cmd_sensor_read),
	SHELL_CMD(info, NULL, "Print device name and ready state", cmd_sensor_info),
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sub_sensor, "LED sensor commands", NULL);
