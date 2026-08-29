#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/led_strip.h>

#define SLEEP_TIME_MS 1000

/* The devicetree node identifier for the "led0" alias. */
#define STRIP_NODE DT_ALIAS(led_strip)
const struct device * strip = DEVICE_DT_GET(STRIP_NODE);

#define STRIP_NUM_PIXELS 1
struct led_rgb pixels[STRIP_NUM_PIXELS];

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = true;

    if (!device_is_ready(strip)) return 0;

 // Set the first pixel to solid Red
    pixels[0].r = 255;
    pixels[0].g = 0;
    pixels[0].b = 0;

    // Flush color array updates to the NeoPixels
    led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);


    while (1) {
        led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);

        led_state = !led_state;

        pixels[0].r = led_state?127:0;
        pixels[0].g = 0;
        pixels[0].b = 0;

        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        k_msleep(SLEEP_TIME_MS);
    }
    return 0;
}
