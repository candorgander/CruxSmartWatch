#include <Arduino.h>
#include <lvgl.h>

#if LV_USE_TFT_ESPI
#include <TFT_eSPI.h>
#endif

#include "dp1.h"
#include "Builder.h"

#include "RTOS.h"

/*Set to your screen resolution and rotation*/
#define TFT_HOR_RES 240
#define TFT_VER_RES 280
#define TFT_ROTATION LV_DISPLAY_ROTATION_0

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

lv_display_t *disp;

// /* LVGL calls it when a rendered image needs to copied to the display*/
// void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
// {
//     /*Copy `px map` to the `area`*/

//     /*For example ("my_..." functions needs to be implemented by you)
//     uint32_t w = lv_area_get_width(area);
//     uint32_t h = lv_area_get_height(area);

//     my_set_window(area->x1, area->y1, w, h);
//     my_draw_bitmaps(px_map, w * h);
//      */

//     /*Call it to tell LVGL you are ready*/
//     lv_display_flush_ready(disp);
// }

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
}

void startLVGLTask()
{
    xTaskCreate(displayTask, "Display Task", 8096, NULL, 5, NULL);
}