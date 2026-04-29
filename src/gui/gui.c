#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "gui.h"
#include "lvgl/lvgl.h"
#include "model.h"
#include "squareline/ui.h"

display_model_t g_model;

/* ================== IPC Thread ================== */

#define SOCKET_PATH "/tmp/lafvin_display.sock"

pthread_t sensors_thread_id;
pthread_mutex_t g_model_mutex = PTHREAD_MUTEX_INITIALIZER;

int gui_connect_to_sensors(void)
{
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    while (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        sleep(1);
    }

    return fd;
}

void *sensors_thread(void *arg)
{
    (void)arg;

    int sock = gui_connect_to_sensors();
    char buf[DISPLAY_MODEL_JSON_SIZE];

    while (1) {
        int len = recv(sock, buf, sizeof(buf) - 1, 0);
        if (len <= 0)
            continue;

        buf[len] = '\0';

        display_model_t tmp;
        display_model_from_json(buf, &tmp);

        pthread_mutex_lock(&g_model_mutex);
        g_model = tmp;
        pthread_mutex_unlock(&g_model_mutex);
    }
}

/* ======================================================= */

/* ================== SquareLine Studio ================== */

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

    lv_chart_set_range(ui_TemperatureChart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);

    /* ================= HUMIDITY ================= */
    lv_chart_set_point_count(ui_HumidityChart, CHART_POINTS);

    hum_series = lv_chart_add_series(ui_HumidityChart, lv_palette_main(LV_PALETTE_GREEN),
                                     LV_CHART_AXIS_PRIMARY_Y);

    lv_chart_set_range(ui_HumidityChart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);

    /* ================= PRESSURE ================= */
    lv_chart_set_point_count(ui_PressureChart, CHART_POINTS);

    press_series = lv_chart_add_series(ui_PressureChart, lv_palette_main(LV_PALETTE_ORANGE),
                                       LV_CHART_AXIS_PRIMARY_Y);

    lv_chart_set_range(ui_PressureChart, LV_CHART_AXIS_PRIMARY_Y, 0, 1200);

    /* Initial refresh */
    lv_chart_refresh(ui_TemperatureChart);
    lv_chart_refresh(ui_HumidityChart);
    lv_chart_refresh(ui_PressureChart);
}

void gui_update_system(void)
{
    /* Time */
    struct tm timeinfo;
    localtime_r(&g_model.timestamp, &timeinfo);
    char time_str[64];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", &timeinfo);
    char date_str[64];
    strftime(date_str, sizeof(date_str), "%Y-%m-%d", &timeinfo);
    lv_label_set_text(ui_TimeLabel, time_str);
    lv_label_set_text(ui_DateLabel, date_str);

    /* ================= WIFI ================= */
    if (g_model.wifi_connected) {
        lv_label_set_text(ui_WifiStatus, "WiFi: Connected");
        lv_label_set_text_fmt(ui_SSID, "SSID: %s", g_model.wifi_ssid);
    } else {
        lv_label_set_text(ui_WifiStatus, "WiFi: Disconnected");
        lv_label_set_text_fmt(ui_SSID, "SSID: Not connected");
    }

    if (g_model.ip[0] != '\0') {
        lv_label_set_text_fmt(ui_IpAddress, "IP: %s", g_model.ip);
    } else {
        lv_label_set_text(ui_IpAddress, "IP: Not connected");
    }

    /* ================= SYSTEM ================= */
    lv_label_set_text_fmt(ui_CpuTemp, "CPU Temp: %d°C", g_model.cpu_temp);
    lv_label_set_text_fmt(ui_CpuLoad, "CPU Load: %d%%", g_model.cpu_load);
    lv_label_set_text_fmt(ui_RamUsage, "RAM Usage: %d%%(%ld MB)", g_model.ram_usage_pr,
                          g_model.ram_total);
    lv_label_set_text_fmt(ui_UpTime, "Uptime: %ld s", g_model.uptime);

    /* ================= WEB PAGE ================= */
    if (g_model.ip[0] != '\0') {
        /* TODO: not hard-coded */
        lv_label_set_text_fmt(ui_WebPage, "http://%s:3000", g_model.ip);
    } else {
        lv_label_set_text(ui_WebPage, "Web page: Not available");
    }
}

void gui_update_sensors(lv_timer_t *timer)
{
    (void)timer;

    /* ================= CHARTS ================= */

    // Temperature
    if (g_model.has_bmp280)
        lv_chart_set_next_value(ui_TemperatureChart, temp_series_1,
                                (lv_coord_t)g_model.bmp280_temperature);

    if (g_model.has_htu21d)
        lv_chart_set_next_value(ui_TemperatureChart, temp_series_2,
                                (lv_coord_t)g_model.htu21d_temperature);

    // Humidity
    if (g_model.has_htu21d)
        lv_chart_set_next_value(ui_HumidityChart, hum_series, (lv_coord_t)g_model.htu21d_humidity);

    // Pressure
    if (g_model.has_bmp280)
        lv_chart_set_next_value(ui_PressureChart, press_series,
                                (lv_coord_t)g_model.bmp280_pressure);

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

    pthread_create(&sensors_thread_id, NULL, sensors_thread, NULL);
}
