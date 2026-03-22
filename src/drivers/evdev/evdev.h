#ifndef PI_HOME_LAFVIN_UI_EVDEV_H
#define PI_HOME_LAFVIN_UI_EVDEV_H

#include "lvgl/lvgl.h"
#include "lvgl_driver.h"

int lvgl_evdev_init(lvgl_driver_t *driver);
void lvgl_evdev_deinit(lv_indev_t *indev);

#endif /* PI_HOME_LAFVIN_UI_EVDEV_H */
