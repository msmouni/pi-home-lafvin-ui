#ifndef PI_HOME_LAFVIN_UI_DISPLAY_H
#define PI_HOME_LAFVIN_UI_DISPLAY_H

#include "lvgl/lvgl.h"

/**
 * Initialize the display driver
 *
 * @return the LVGL display
 */
lv_display_t *lvgl_display_init(void);

/**
 * The run loop of the display driver
 */
void lvgl_display_run_loop(void);

void lvgl_display_deinit(lv_display_t *display);

#endif /* PI_HOME_LAFVIN_UI_DISPLAY_H */