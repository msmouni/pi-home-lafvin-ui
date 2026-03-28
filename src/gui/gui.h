#ifndef PI_HOME_LAFVIN_UI_GUI_H
#define PI_HOME_LAFVIN_UI_GUI_H

#include "lvgl.h"
#include "ui.h"
#include <string.h>
#include <time.h>

// TMP: to take from pi-home-display package later
typedef struct { /* Sensors data */
    bool has_bmp280;
    float bmp280_temperature;
    float bmp280_pressure;
    bool has_htu21d;
    float htu21d_temperature;
    float htu21d_humidity; /* System info */
    bool wifi_connected;
    char ip[128];
    time_t timestamp;
    int cpu_temp;
    int cpu_load;
} display_model_t;

void gui_create(void);

void gui_update(const display_model_t *model);

#endif /* PI_HOME_LAFVIN_UI_GUI_H */
