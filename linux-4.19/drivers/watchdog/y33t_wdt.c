#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/watchdog.h>
#include <linux/i2c.h>

#define Y33T_ADDR_CMD 0
#define Y33T_CMD_PING 13
#define Y33T_ADDR_BOOT_REASON 1

struct y33t_wdt {
	struct watchdog_device wdt;
	struct i2c_client *i2c_client;
};

static int y33t_wdt_ping(struct watchdog_device *wdt)
{
	struct i2c_client *client = to_i2c_client(wdt->parent);
	i2c_smbus_write_word_data(client, Y33T_ADDR_CMD, Y33T_CMD_PING);
	return 0;
}

static const struct watchdog_ops y33t_wdt_ops = {
	.owner = THIS_MODULE,
	.start = y33t_wdt_ping,
	.ping = y33t_wdt_ping,
};

static const struct watchdog_info y33t_wdt_info = {
	.options = WDIOF_KEEPALIVEPING,
	.identity = KBUILD_MODNAME,
};

static int y33t_wdt_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret;
	struct y33t_wdt *drv_data;

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_BYTE_DATA))
		return -ENODEV;

	drv_data = devm_kzalloc(&client->dev, sizeof(struct y33t_wdt), GFP_KERNEL);
	if (!drv_data)
		return -ENOMEM;
	drv_data->wdt.ops = &y33t_wdt_ops;
	drv_data->wdt.info = &y33t_wdt_info;
	drv_data->wdt.max_hw_heartbeat_ms = 3000;
	drv_data->wdt.parent = &client->dev;

	watchdog_init_timeout(&drv_data->wdt, 0, &client->dev);
	watchdog_set_drvdata(&drv_data->wdt, drv_data);

	ret = i2c_smbus_read_byte_data(client, Y33T_ADDR_BOOT_REASON);
	if (ret == 1)
		drv_data->wdt.bootstatus |= WDIOF_CARDRESET;
	else if (ret == 2)
		drv_data->wdt.bootstatus |= WDIOF_OVERHEAT;
	if (ret != 0)
		dev_info(&client->dev, "unclean boot\n");

	ret = watchdog_register_device(&drv_data->wdt);
	if (ret) {
		dev_err(&client->dev, "watchdog_register_device failed\n");
		return ret;
	}

	return 0;
}

static const struct i2c_device_id y33t_wdt_idtable[] = {
	{ "y33t_wdt", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, y33t_wdt_idtable);

static const struct of_device_id y33t_wdt_of_table[] = {
	{ .compatible = "omega,y33t_wdt" },
	{ },
};
MODULE_DEVICE_TABLE(of, y33t_wdt_of_table);

static struct i2c_driver y33t_wdt_driver = {
	.driver = {
		.name = KBUILD_MODNAME,
		.of_match_table = of_match_ptr(y33t_wdt_of_table),
	},
	.probe = y33t_wdt_probe,
	.id_table = y33t_wdt_idtable
};

module_i2c_driver(y33t_wdt_driver);

MODULE_DESCRIPTION("Y33T Watchdog driver");
MODULE_AUTHOR("Edmund Huber <me@ehuber.info>");
