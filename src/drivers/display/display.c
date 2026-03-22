#include "display.h"
#include "fbdev.h"

lv_display_t *lvgl_display_init(void)
{
    return lvgl_fbdev_init();
}

void lvgl_display_run_loop(void)
{
    lvgl_fbdev_run_loop();
}

void lvgl_display_deinit(lv_display_t *display)
{
    lv_display_delete(display);
}