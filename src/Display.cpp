#include <Arduino.h>
#include <lvgl.h>

#if LV_USE_TFT_ESPI
#include <TFT_eSPI.h>
#endif

// #include "dp1.h"
// #include "Builder.h"

#include "UI/UI.h"
#include "Touch.h"


/*Set to your screen resolution and rotation*/
#define TFT_HOR_RES 240
#define TFT_VER_RES 280
#define TFT_ROTATION LV_DISPLAY_ROTATION_0

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

lv_display_t *disp;

lv_indev_t *touchpad;
static lv_key_t lastKey = LV_KEY_NEXT;

SemaphoreHandle_t lvgl_mutex;

void setupRTOS()
{
    lvgl_mutex = xSemaphoreCreateMutex();
}

bool takeLVGLMutex()
{
    return xSemaphoreTake(lvgl_mutex, portMAX_DELAY);
}

bool relaseLVGLMutex()
{
    return xSemaphoreGive(lvgl_mutex);
}

/*use Arduinos millis() as tick source*/
static uint32_t my_tick(void)
{
    return millis();
}

void displayTask(void *pvparameters)
{
    while (1)
    {
        if (takeLVGLMutex())
        {
            lv_timer_handler();
            relaseLVGLMutex();
        }
        vTaskDelay(50);
    }
}

void touchcb(lv_indev_t *indev, lv_indev_data_t *data)
{
    bool next = keystate(LV_KEY_NEXT);
    bool prev = keystate(LV_KEY_PREV);

    if (next)
    {
        data->key = LV_KEY_NEXT;
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else if (prev)
    {
        data->key = LV_KEY_PREV;
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else
    {
        data->key = lastKey; // hold the last key during release
        data->state = LV_INDEV_STATE_RELEASED;
    }

    if (data->state == LV_INDEV_STATE_PRESSED)
        lastKey = (lv_key_t)data->key;
}

void setupTouchIndev()
{
    takeLVGLMutex();

    touchpad = lv_indev_create();
    lv_indev_set_type(touchpad, LV_INDEV_TYPE_KEYPAD);
    lv_indev_set_read_cb(touchpad, touchcb);
    lv_indev_set_disp(touchpad, disp);

    relaseLVGLMutex();
}

lv_indev_t* getTouchpad()
{
    return touchpad;
}

lv_display_t* getDisplay()
{
    return disp;
}

void setupDisplay()
{
    setupRTOS();

    pinMode(TFT_RST, OUTPUT);
    digitalWrite(TFT_RST, HIGH);
    delay(10);
    digitalWrite(TFT_RST, LOW);
    delay(10);
    digitalWrite(TFT_RST, HIGH);
    delay(120);

    lv_init();

    /*Set a tick source so that LVGL will know how much time elapsed. */
    lv_tick_set_cb(my_tick);

    /*TFT_eSPI can be enabled lv_conf.h to initialize the display in a simple way*/
    disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, sizeof(draw_buf));
    lv_display_set_rotation(disp, TFT_ROTATION);

    setupTouchIndev();
}

void startLVGLTask()
{
    xTaskCreate(displayTask, "Display Task", 8096, NULL, 5, NULL);
}