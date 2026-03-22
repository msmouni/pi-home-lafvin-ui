#ifndef PI_HOME_LAFVIN_UI_DRIVER_H
#define PI_HOME_LAFVIN_UI_DRIVER_H

#include "lvgl/lvgl.h"

typedef struct {
    lv_display_t *display;
    lv_indev_t *input;
} lvgl_driver_t;

lvgl_driver_t *lvgl_driver_create(void);
void lvgl_driver_run(lvgl_driver_t *drv);
void lvgl_driver_destroy(lvgl_driver_t *drv);

#endif /* PI_HOME_LAFVIN_UI_DRIVER_H */