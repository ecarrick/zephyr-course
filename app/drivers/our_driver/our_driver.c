#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include "our_driver.h"

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

struct our_driver_data {
    uint8_t counter;
    uint8_t step;
};

struct our_driver_config {
    uint8_t step;
    struct gpio_dt_spec led_gpio;
};

int our_driver_restart_count(const struct device *dev) {
    struct our_driver_data *data = dev->data;
    data->counter = 0;
    return 0;
}

int our_driver_set_skip(const struct device *dev, uint8_t new_step) {
    struct our_driver_data *data = dev->data;
    data->step = new_step;
    return 0;
}


static int channel_get_my_impl(const struct device *dev, 
                               enum sensor_channel chan,
                               struct sensor_value *val)
{
    // should turn off the LED
    const struct our_driver_config *cfg = dev->config;
    gpio_pin_set_dt(&cfg->led_gpio, 0);
    LOG_INF("Hello from channel get %d", chan);
    return 0;
}

static int sample_fetch_my_impl(const struct device *dev,
				     enum sensor_channel chan) {
    // should turn on the LED
    const struct our_driver_config *cfg = dev->config;

    gpio_pin_set_dt(&cfg->led_gpio, 1);
    struct our_driver_data *data = dev->data;
    data->counter = (data->counter < 255) ? data->counter+1 : 255;
    LOG_INF("LED SHOULD BE ON - count %d", data->counter);

    return 0;
}


static DEVICE_API(sensor, api_my_driver) = {
    .channel_get = channel_get_my_impl,
    .sample_fetch = sample_fetch_my_impl,
};

static int our_driver_init(const struct device* dev) {
    const struct our_driver_config *cfg = dev->config;

    if(!gpio_is_ready_dt(&cfg->led_gpio)) {
        return -ENODEV;
    }

    
    
    LOG_INF("device initialized");

    return gpio_pin_configure_dt(&cfg->led_gpio, GPIO_OUTPUT_INACTIVE);;
}

#define OUR_DRIVER_DEFINE(inst)                           \
    static struct our_driver_data data_##inst;            \
    static const struct our_driver_config cfg_##inst = {  \
        .step = DT_INST_PROP(inst, step),             \
        .led_gpio = GPIO_DT_SPEC_INST_GET(inst, gpios) \
    };                                                    \
    DEVICE_DT_INST_DEFINE(inst,                           \
        our_driver_init, NULL,                            \
        &data_##inst, &cfg_##inst,                        \
        POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY,         \
        &api_my_driver)
        
DT_INST_FOREACH_STATUS_OKAY(OUR_DRIVER_DEFINE)