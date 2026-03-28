#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "gui.h"
#include "lvgl_driver.h"

void fake_data(display_model_t *m)
{
    m->wifi_connected = true;
    strcpy(m->ip, "192.168.1.10");

    m->cpu_load = rand() % 100;
    m->cpu_temp = 40 + rand() % 20;

    m->has_bmp280 = true;
    m->bmp280_temperature = 20 + rand() % 10;
    m->bmp280_pressure = 1000 + rand() % 50;

    m->has_htu21d = true;
    m->htu21d_temperature = 21 + rand() % 10;
    m->htu21d_humidity = 40 + rand() % 20;

    /////// Time and date
    m->timestamp = time(NULL);
}

/* =========================
 * MAIN
 * ========================= */

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

    display_model_t display_model;

    // lvgl_driver_run(lvgl_drv);
    uint32_t idle_time;

    /* Handle LVGL tasks */
    while (true) {
        fake_data(&display_model);
        gui_update(&display_model);

        /* Returns the time to the next timer execution */
        idle_time = lv_timer_handler();
        usleep(idle_time * 1000);
    }

    return 0;
}
