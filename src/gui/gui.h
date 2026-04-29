#ifndef PI_HOME_LAFVIN_UI_GUI_H
#define PI_HOME_LAFVIN_UI_GUI_H

#include "lvgl.h"
#include "ui.h"
#include <string.h>
#include <time.h>

#define GUI_UPDATE_INTERVAL_MS 5000

void gui_create(void);

void gui_update_system(void);

void gui_update_sensors(lv_timer_t *timer);

#endif /* PI_HOME_LAFVIN_UI_GUI_H */
