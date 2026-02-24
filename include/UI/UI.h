#pragma once

#include <lvgl.h>

// Page draw Functions
#include "Statusbar.h"

#define BACKGROUND lv_color_hex(0x110A1F)
#define PRIMARY lv_color_hex(0xb794de)
#define SECONDARY lv_color_hex(0x852956)
#define TEXT_COLOR lv_color_hex(0xe7dcf4)
#define ACCENT lv_color_hex(0xc84f62)

void drawUI();
