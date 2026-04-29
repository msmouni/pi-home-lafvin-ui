#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "gui.h"
#include "lvgl_driver.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    /* Initialize LVGL */
    lv_init();

    lvgl_driver_t *lvgl_drv = lvgl_driver_create();
    if (!lvgl_drv)
        return -1;

    gui_create();

    lv_timer_create(gui_update_sensors, GUI_UPDATE_INTERVAL_MS, NULL);

    uint32_t idle_time;

    /* Handle LVGL tasks */
    while (true) {
        gui_update_system();

        /* Returns the time to the next timer execution */
        idle_time = lv_timer_handler();
        usleep(idle_time * 1000);
    }

    return 0;
}
