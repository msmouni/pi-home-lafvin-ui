#include <stdio.h>
#include <stdlib.h>

#include "gui.h"
#include "lvgl/lvgl.h"

static void btn_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        printf("Button clicked!\n");
    }
}

void gui_create(void)
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
