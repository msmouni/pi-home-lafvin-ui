#include <stdio.h>
#include <stdlib.h>

#include "gui.h"
#include "lvgl_driver.h"

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

    lvgl_driver_run(lvgl_drv);

    return 0;
}
