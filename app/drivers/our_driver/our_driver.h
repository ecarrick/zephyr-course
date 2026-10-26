#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>


#ifdef __cplusplus
extern "C" {
#endif

int our_driver_restart_count(const struct device *dev);

#ifdef __cplusplus
}
#endif
