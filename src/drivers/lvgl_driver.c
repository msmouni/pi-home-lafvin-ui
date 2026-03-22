#include <stdio.h>
#include <stdlib.h>

#include "display.h"
#include "evdev.h"
#include "lvgl_driver.h"

lvgl_driver_t *lvgl_driver_create(void)
{
    lvgl_driver_t *drv = malloc(sizeof(*drv));
    if (!drv)
        return NULL;

    drv->display = lvgl_display_init();
    if (!drv->display) {
        fprintf(stderr, "Failed to initialize display device\n");
        goto error_display;
    }

    if (lvgl_evdev_init(drv) != 0) {
        fprintf(stderr, "Failed to initialize input device\n");
        goto error_input;
    }

    return drv;

error_input:
    lvgl_display_deinit(drv->display);
    free(drv);
    return NULL;

error_display:
    free(drv);
    return NULL;
}

void lvgl_driver_run(lvgl_driver_t *drv)
{
    if (!drv)
        return;

    lvgl_display_run_loop();
}

void lvgl_driver_destroy(lvgl_driver_t *drv)
{
    if (!drv)
        return;

    if (drv->input)
        lvgl_evdev_deinit(drv->input);

    if (drv->display)
        lvgl_display_deinit(drv->display);

    free(drv);
}
