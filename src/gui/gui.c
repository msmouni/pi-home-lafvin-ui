#include <stdio.h>
#include <stdlib.h>

#include "gui.h"
#include "lvgl/lvgl.h"
#include "squareline/ui.h"

static void btn_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        printf("Button clicked!\n");
    }
}

static void gui_create_v0(void)
{
    lv_obj_t *scr = lv_screen_active();

    /* Create tabview */
    lv_obj_t *tabview = lv_tabview_create(scr);
    lv_obj_set_size(tabview, LV_PCT(100), LV_PCT(100));

    /* Tabs */
    lv_obj_t *tab_status = lv_tabview_add_tab(tabview, "Status");
    lv_obj_t *tab_ctrl = lv_tabview_add_tab(tabview, "Control");

    /* ===== Status tab ===== */
    lv_obj_t *label_title = lv_label_create(tab_status);
    lv_label_set_text(label_title, "Raspberry Pi Status");
    lv_obj_set_style_text_font(label_title, &lv_font_montserrat_20, 0);
    lv_obj_align(label_title, LV_ALIGN_TOP_MID, 0, 10);

    lv_obj_t *label_ip = lv_label_create(tab_status);
    lv_label_set_text(label_ip, "IP: 192.168.1.100");
    lv_obj_align(label_ip, LV_ALIGN_TOP_LEFT, 10, 50);

    lv_obj_t *label_info = lv_label_create(tab_status);
    lv_label_set_text(label_info, "Display: SPI LCD\nTouch: ADS7846");
    lv_obj_align(label_info, LV_ALIGN_TOP_LEFT, 10, 80);

    /* ===== Control tab ===== */
    lv_obj_t *btn = lv_button_create(tab_ctrl);
    lv_obj_set_size(btn, 120, 50);
    lv_obj_center(btn);
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Press me");
    lv_obj_center(btn_label);
}

/////////////////////////// SquareLine Studio

#define CHART_POINTS 60 // 60 samples (e.g. 60 seconds)

// Temperature (2 series)
static lv_chart_series_t *temp_series_1; // BMP280
static lv_chart_series_t *temp_series_2; // HTU21D

// Humidity
static lv_chart_series_t *hum_series;

// Pressure
static lv_chart_series_t *press_series;

void gui_post_init(void)
{
    /* ================= TEMPERATURE ================= */
    lv_chart_set_point_count(ui_TemperatureChart, CHART_POINTS);

    temp_series_1 = lv_chart_add_series(ui_TemperatureChart, lv_palette_main(LV_PALETTE_RED),
                                        LV_CHART_AXIS_PRIMARY_Y);

    temp_series_2 = lv_chart_add_series(ui_TemperatureChart, lv_palette_main(LV_PALETTE_BLUE),
                                        LV_CHART_AXIS_PRIMARY_Y);

    lv_chart_set_range(ui_TemperatureChart, LV_CHART_AXIS_PRIMARY_Y, -10, 50);

    /* ================= HUMIDITY ================= */
    lv_chart_set_point_count(ui_HumidityChart, CHART_POINTS);

    hum_series = lv_chart_add_series(ui_HumidityChart, lv_palette_main(LV_PALETTE_GREEN),
                                     LV_CHART_AXIS_PRIMARY_Y);

    lv_chart_set_range(ui_HumidityChart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);

    /* ================= PRESSURE ================= */
    lv_chart_set_point_count(ui_PressureChart, CHART_POINTS);

    press_series = lv_chart_add_series(ui_PressureChart, lv_palette_main(LV_PALETTE_ORANGE),
                                       LV_CHART_AXIS_PRIMARY_Y);

    lv_chart_set_range(ui_PressureChart, LV_CHART_AXIS_PRIMARY_Y, 900, 1100);

    /* Initial refresh */
    lv_chart_refresh(ui_TemperatureChart);
    lv_chart_refresh(ui_HumidityChart);
    lv_chart_refresh(ui_PressureChart);
}

void gui_update(const display_model_t *model)
{
    if (!model)
        return;

    /* Time */
    struct tm timeinfo;
    localtime_r(&model->timestamp, &timeinfo);
    char time_str[64];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", &timeinfo);
    char date_str[64];
    strftime(date_str, sizeof(date_str), "%Y-%m-%d", &timeinfo);
    lv_label_set_text(ui_TimeLabel, time_str);
    lv_label_set_text_fmt(ui_DateLabel, date_str);

    /* ================= WIFI ================= */
    if (model->wifi_connected) {
        lv_label_set_text(ui_WifiStatus, "WiFi: Connected");
    } else {
        lv_label_set_text(ui_WifiStatus, "WiFi: Disconnected");
    }

    lv_label_set_text_fmt(ui_IpAddress, "IP: %s", model->ip);

    /* ================= SYSTEM ================= */
    lv_label_set_text_fmt(ui_CpuTemp, "CPU Temp: %d°C", model->cpu_temp);
    lv_label_set_text_fmt(ui_CpuLoad, "CPU Load: %d%%", model->cpu_load);

    /* ================= CHARTS ================= */

    // Temperature
    if (model->has_bmp280)
        lv_chart_set_next_value(ui_TemperatureChart, temp_series_1,
                                (lv_coord_t)model->bmp280_temperature);

    if (model->has_htu21d)
        lv_chart_set_next_value(ui_TemperatureChart, temp_series_2,
                                (lv_coord_t)model->htu21d_temperature);

    // Humidity
    if (model->has_htu21d)
        lv_chart_set_next_value(ui_HumidityChart, hum_series, (lv_coord_t)model->htu21d_humidity);

    // Pressure
    if (model->has_bmp280)
        lv_chart_set_next_value(ui_PressureChart, press_series, (lv_coord_t)model->bmp280_pressure);

    /* Refresh charts */
    lv_chart_refresh(ui_TemperatureChart);
    lv_chart_refresh(ui_HumidityChart);
    lv_chart_refresh(ui_PressureChart);
}

void gui_create(void)
{
    // SquareLine Studio generated code
    ui_init();

    gui_post_init();
}