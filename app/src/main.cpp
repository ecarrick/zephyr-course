#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/led_strip.h>

#ifdef CONFIG_LED_SUBSYSTEM
#ifdef CONFIG_BLINK_SLEEP_250MS
#define SLEEP_TIME_MS 250
#endif
#ifdef CONFIG_BLINK_SLEEP_1000MS
#define SLEEP_TIME_MS 1000
#endif
#ifdef CONFIG_BLINK_SLEEP_2000MS
#define SLEEP_TIME_MS 2000
#endif
#endif


#ifndef SLEEP_TIME_MS
#error "No sleep time for app"
#endif

/* The devicetree node identifier for the "led0" alias. */
#define STRIP_NODE DT_ALIAS(led_strip)
const struct device * strip = DEVICE_DT_GET(STRIP_NODE);

#define STRIP_NUM_PIXELS 1
struct led_rgb pixels[STRIP_NUM_PIXELS];

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static uint8_t get_led_level(void) {
#ifdef CONFIG_LED_ADVANCED
    uint16_t tmp = (uint16_t)255*(uint16_t)CONFIG_LED_BRIGHTNESS;
    return (uint8_t)(tmp/100);
#else
    return 255;
#endif
}

int main(void)
{
    bool led_state = true;

    if (!device_is_ready(strip)) return 0;

 // Set the first pixel to solid Red
    pixels[0].r = get_led_level();
    pixels[0].g = 0;
    pixels[0].b = 0;

    // Flush color array updates to the NeoPixels
    led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);


    while (1) {

        led_state = !led_state;

#ifdef CONFIG_LED_ADVANCED
        if (led_state) {
            pixels[0].r = led_state?get_led_level():0;
            pixels[0].g = 0;
            pixels[0].b = 0;
        } else {
            uint16_t fade_time = CONFIG_LED_FADE_DURATION_MS;
            uint8_t level = get_led_level();
            uint8_t fade_level_step = (level*100) / (fade_time);
            LOG_INF("time: %u level %u step %u", fade_time, level, fade_level_step);
            while (fade_time) {
                level -= fade_level_step;
                LOG_INF("new level %u", level);
                pixels[0].r = level;
                pixels[0].g = 0;
                pixels[0].b = 0;

                led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);

                k_msleep(100);
                if (fade_time > 100) {
                    fade_time -= 100;
                } else {
                    fade_time = 0;
                }
            }


        }

#else

        pixels[0].r = led_state?get_led_level():0;
        pixels[0].g = 0;
        pixels[0].b = 0;

        led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);

#endif
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        k_msleep(SLEEP_TIME_MS);
    }
    return 0;
}
