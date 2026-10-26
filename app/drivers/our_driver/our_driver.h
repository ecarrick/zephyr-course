#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>


#ifdef __cplusplus
extern "C" {
#endif

int our_driver_restart_count(const struct device *dev);
int our_driver_set_skip(const struct device *dev, uint8_t new_step);

#ifdef __cplusplus
}
#endif
