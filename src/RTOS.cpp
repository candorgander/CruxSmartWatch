#include <Arduino.h>

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