#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lvgl/lvgl.h"

#include "src/lib/driver_backends.h"

/* =========================
 * GUI CREATION
 * ========================= */

static void btn_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        printf("Button clicked!\n");
    }
}

static void create_gui(void)
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

/* =========================
 * MAIN
 * ========================= */

int main(int argc, char **argv)
{
    driver_backends_register();

    /* Initialize LVGL */
    lv_init();

    /* Initialize framebuffer / DRM / etc */
    if (driver_backends_init_backend("FBDEV") == -1)
    {
        fprintf(stderr, "Failed to init display backend\n");
        return -1;
    }

    if (driver_backends_init_backend("EVDEV") == -1)
    {
        fprintf(stderr, "Failed to init evdev\n");
        return -1;
    }

    /* Create your GUI */
    create_gui();

    /* Run backend loop (blocking) */
    driver_backends_run_loop();

    return 0;
}
