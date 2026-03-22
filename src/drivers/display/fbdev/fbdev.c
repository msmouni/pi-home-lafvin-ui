#include <unistd.h>

#include "fbdev.h"

// TODO: Make this configurable
#define DISPLAY_DEVICE "/dev/fb1"

lv_display_t *lvgl_fbdev_init(void)
{
    const char *device = DISPLAY_DEVICE;
    lv_display_t *disp = lv_linux_fbdev_create();

    if (disp == NULL)
    {
        return NULL;
    }

    lv_linux_fbdev_set_file(disp, device);

    return disp;
}

void lvgl_fbdev_run_loop(void)
{
    uint32_t idle_time;

    /* Handle LVGL tasks */
    while (true)
    {

        /* Returns the time to the next timer execution */
        idle_time = lv_timer_handler();
        usleep(idle_time * 1000);
    }
}