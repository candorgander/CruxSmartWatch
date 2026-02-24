#pragma once

void setupRTOS();
bool takeLVGLMutex();
bool relaseLVGLMutex();

void setupDisplay();
void startLVGLTask();

lv_indev_t* getTouchpad();
lv_display_t* getDisplay();