#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

void board_late_init_hook(void) {
    printk("Board Initialized\n");
}