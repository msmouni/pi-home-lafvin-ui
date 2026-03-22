#ifndef PI_HOME_LAFVIN_UI_EVDEV_H
#define PI_HOME_LAFVIN_UI_EVDEV_H

#include "lvgl/lvgl.h"

/**
 * Initialize evdev pointer device
 *
 * @param display LVGL display
 * @return lv_indev_t* or NULL on failure
 */
lv_indev_t *lvgl_evdev_init(lv_display_t *display);
void lvgl_evdev_deinit(lv_indev_t *indev);

#endif /* PI_HOME_LAFVIN_UI_EVDEV_H */
