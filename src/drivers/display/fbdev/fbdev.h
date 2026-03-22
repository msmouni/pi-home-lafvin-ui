#ifndef PI_HOME_LAFVIN_UI_FBDEV_H
#define PI_HOME_LAFVIN_UI_FBDEV_H

#include "lvgl/lvgl.h"

/**
 * Initialize the fbdev driver
 *
 * @return the LVGL display
 */
lv_display_t *lvgl_fbdev_init(void);

/**
 * The run loop of the fbdev driver
 */
void lvgl_fbdev_run_loop(void);

#endif /* PI_HOME_LAFVIN_UI_FBDEV_H */