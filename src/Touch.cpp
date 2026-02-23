#include <Arduino.h>
#include <lvgl.h>

#include "pindef.h"
#include "Touch.h"

int touchstate[MAX_TOUCH] = {0};
int touchpin[MAX_TOUCH] = {TOUCH_TL, TOUCH_TR};

#define NEXT_KEY 1
#define PREV_KEY 0

int keystate(uint32_t k)
{
    switch (k)
    {
    case LV_KEY_PREV:
        return digitalRead(touchpin[PREV_KEY]);
        break;
    case LV_KEY_NEXT:
        return digitalRead(touchpin[NEXT_KEY]);
        break;
    }
    return -1;
}

lv_key_t getKeyPressed()
{
    for (int i = 0; i < MAX_TOUCH; i++)
    {
        touchstate[i] = digitalRead(touchpin[i]);
    }

    if (touchstate[NEXT_KEY])
    {
        // Serial.println("Prev Key Pressed");
        return LV_KEY_NEXT;
    }
    else if (touchstate[PREV_KEY])
    {
        // Serial.println("Next Key Pressed");
        return LV_KEY_PREV;
    }
    return LV_KEY_NEXT;
}

void setupTouch()
{
    pinMode(TOUCH_TL, INPUT);
    pinMode(TOUCH_TR, INPUT);
}