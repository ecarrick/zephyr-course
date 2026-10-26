#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>

#include "our_driver.h"

const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));

static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv) {
    int ret = sensor_sample_fetch(driver);
    shell_print(sh, "sensor_sample_fetch ret %d\n", ret);
    return 0;
}

static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv) {
    struct sensor_value val;
    int ret = sensor_channel_get(driver, SENSOR_CHAN_ALL, &val);
     shell_print(sh, "channel ret %d\n", ret);
    return 0;
}


static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv) {
    shell_print(sh, "name %s - ready %d\n", driver->name, driver->state->initialized);
    return 0;
}

static int cmd_sensor_set(const struct shell *sh, size_t argc, char **argv) {
    int ret = 0;
    if (argc < 2) {
        shell_error(sh, "Not enough arguments\n");
        return -1;
    }
    unsigned long int val = strtoul(argv[1], NULL, 10);
    if (val > 0 && val < UINT8_MAX) {
        ret = our_driver_set_skip(driver, (uint8_t)val);
    } else {
        shell_error(sh, "provided argument out of range\n");
        return -ERANGE;
    }
    if (0 != ret) {
        return ret;
    }
    shell_print(sh, "name %s - ready %d\n", driver->name, driver->state->initialized);
    return 0;
}

/* Create subcommands array for "demo" */
SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
    SHELL_CMD(fetch, NULL, "execute fetch", cmd_sensor_fetch),
    SHELL_CMD(read,   NULL, "execute ready from sensor", cmd_sensor_read),
    SHELL_CMD(info,   NULL, "print driver info", cmd_sensor_info),
    SHELL_CMD_ARG(set, NULL, "set driver skip value", cmd_sensor_set, 2, 2),
    SHELL_SUBCMD_SET_END);
    
/* Register root command "demo" with subcommands */
SHELL_CMD_REGISTER(sensor, &sub_sensor, "sensor commands", NULL);