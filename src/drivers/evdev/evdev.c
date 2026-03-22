#include "evdev.h"

// TODO: Make this configurable
#define INPUT_DEVICE "/dev/input/event8"

lv_indev_t *lvgl_evdev_init(lv_display_t *display)
{
    const char *input_device = INPUT_DEVICE;

    lv_indev_t *indev = lv_evdev_create(LV_INDEV_TYPE_POINTER, input_device);
    if (!indev)
    {
        LV_LOG_ERROR("Failed to open evdev device: %s", input_device);
        return NULL;
    }

    lv_indev_set_display(indev, display);

    return indev;
}

void lvgl_evdev_deinit(lv_indev_t *indev)
{
    lv_evdev_delete(indev);
}
