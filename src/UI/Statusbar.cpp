#include <Arduino.h>
#include <lvgl.h>

#include "Display.h"
#include "UI/UI.h"

#include "UI/images/bleIcon.h"
#include "UI/images/BattIcon.h"

void drawStatusBar()
{
    takeLVGLMutex();

    static lv_style_t style_mainscreen;
    lv_style_init(&style_mainscreen);
    lv_style_set_bg_color(&style_mainscreen, BACKGROUND);
    lv_obj_add_style(lv_screen_active(), &style_mainscreen, 0);

    lv_obj_t *statusbar = lv_obj_create(lv_layer_top());
    lv_obj_set_size(statusbar, 240, 40);
    lv_obj_set_pos(statusbar, 0, 0);

    static lv_style_t style_statusbar;
    lv_style_init(&style_statusbar);
    lv_style_set_bg_color(&style_statusbar, BACKGROUND);
    lv_style_set_pad_all(&style_statusbar, 0);
    lv_style_set_margin_bottom(&style_statusbar, 0);
    lv_style_set_margin_top(&style_statusbar, 0);
    lv_style_set_margin_left(&style_statusbar, 0);
    lv_style_set_margin_right(&style_statusbar, 0);
    lv_style_set_border_width(&style_statusbar, 0);
    lv_obj_add_style(statusbar, &style_statusbar, 0);

    LV_IMAGE_DECLARE(bleIcon);
    LV_IMAGE_DECLARE(BattIcon);

    static lv_style_t style_img;
    lv_style_init(&style_img);
    lv_style_set_bg_opa(&style_img, LV_OPA_TRANSP);
    lv_style_set_bg_color(&style_img, BACKGROUND);

    lv_obj_t *ble = lv_image_create(statusbar);
    lv_image_set_src(ble, &bleIcon);
    lv_obj_set_pos(ble, 25, 8);
    lv_obj_set_style_bg_color(ble, BACKGROUND, 0);
    lv_obj_add_style(ble, &style_img, 0);

    lv_obj_t *batt = lv_image_create(statusbar);
    lv_image_set_src(batt, &BattIcon);
    lv_obj_set_style_bg_color(batt, BACKGROUND, 0);
    lv_obj_set_pos(batt, 180, 8);
    lv_obj_add_style(batt, &style_img, 0);

    relaseLVGLMutex();
}