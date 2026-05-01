#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "fbdev.h"

#define DISPLAY_FB_NAME "fb_ili9486"

static int find_fb_device(char *device, size_t size)
{
    char fb_name_file[256];
    int found = -1;

    for (int i = 0; i < 5; i++) {
        snprintf(fb_name_file, sizeof(fb_name_file), "/sys/class/graphics/fb%d/name", i);

        if (access(fb_name_file, R_OK) != 0)
            continue;

        FILE *fp = fopen(fb_name_file, "r");
        if (!fp) {
            fprintf(stderr, "Failed to open %s\n", fb_name_file);
            continue;
        }

        char fb_name[256];
        size_t n = fread(fb_name, 1, sizeof(fb_name) - 1, fp);
        fclose(fp);

        if (n == 0)
            continue;

        fb_name[n] = '\0';

        // Strip newline
        if (n > 0 && (fb_name[n - 1] == '\n' || fb_name[n - 1] == '\r')) {
            fb_name[n - 1] = '\0';
        }

        printf("fb%d name: %s\n", i, fb_name);

        if (strstr(fb_name, DISPLAY_FB_NAME)) {
            found = i;
            break;
        }
    }

    if (found < 0) {
        fprintf(stderr, "Framebuffer %s not found\n", DISPLAY_FB_NAME);
        return -1;
    }

    snprintf(device, size, "/dev/fb%d", found);
    return 0;
}

lv_display_t *lvgl_fbdev_init(void)
{
    char device[32];

    if (!find_fb_device(device, sizeof(device))) {
        fprintf(stderr, "Failed to find framebuffer device\n");
        return NULL;
    }

    printf("Using framebuffer device: %s\n", device);

    lv_display_t *disp = lv_linux_fbdev_create();
    if (!disp)
        return NULL;

    lv_linux_fbdev_set_file(disp, device);

    return disp;
}

void lvgl_fbdev_run_loop(void)
{
    uint32_t idle_time;

    /* Handle LVGL tasks */
    while (true) {
        /* Returns the time to the next timer execution */
        idle_time = lv_timer_handler();
        usleep(idle_time * 1000);
    }
}