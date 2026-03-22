#include "evdev.h"
#include <unistd.h>

static void discovery_cb(lv_indev_t *indev, lv_evdev_type_t type, void *user_data)
{
    if (type != LV_EVDEV_TYPE_ABS) {
        LV_LOG_USER("Ignoring non-touch device: %s", type == LV_EVDEV_TYPE_REL   ? "REL"
                                                     : type == LV_EVDEV_TYPE_KEY ? "KEY"
                                                                                 : "unknown");
        return;
    }

    LV_LOG_USER("Touch device found!");

    lvgl_driver_t *driver = user_data;

    lv_indev_set_display(indev, driver->display);
    driver->input = indev;
}

int lvgl_evdev_init(lvgl_driver_t *driver)
{
    if (!driver || !driver->display)
        return -1;

    if (lv_evdev_discovery_start(discovery_cb, driver) != LV_RESULT_OK) {
        return -1;
    }

    return 0;
}

void lvgl_evdev_deinit(lv_indev_t *indev) { lv_evdev_delete(indev); }
