#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>

#include "our_driver.h"

const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));

static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv) {
    int ret = sensor_sample_fetch(driver);
    shell_print(sh, "sensor_sample_fetch ret %d", ret);
    return 0;
}

static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv) {
    struct sensor_value val;
    int ret = sensor_channel_get(driver, SENSOR_CHAN_ALL, &val);
     shell_print(sh, "channel ret %d", ret);
    return 0;
}


static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv) {
    shell_print(sh, "name %s - ready %d", driver->name, driver->state->initialized);
    return 0;
}



/* Create subcommands array for "demo" */
SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
    SHELL_CMD(fetch, NULL, "Print params command.", cmd_sensor_fetch),
    SHELL_CMD(read,   NULL, "Salute command.", cmd_sensor_read),
    SHELL_CMD(info,   NULL, "Salute command.", cmd_sensor_info),
    SHELL_SUBCMD_SET_END);
    
/* Register root command "demo" with subcommands */
SHELL_CMD_REGISTER(sensor, &sub_sensor, "Demo commands", NULL);