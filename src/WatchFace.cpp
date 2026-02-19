#include <Arduino.h>
#include <lvgl.h>

// #include "Builder.h"
#include "dp1.h"
#include "Builder.h"


static lv_style_t main_screen;
static lv_style_t Time;
lv_obj_t *label;
static lv_style_t DP_IMG;
lv_obj_t *img1;
int ct = 11;

void ShowWatchFace()
{

    LV_FONT_DECLARE(Builder);

    
    lv_style_init(&main_screen);
    lv_style_set_bg_color(&main_screen, lv_color_black());
    lv_obj_add_style(lv_screen_active(), &main_screen, 0);
  

    lv_style_init(&Time);
    lv_style_set_text_color(&Time, lv_palette_main(LV_PALETTE_DEEP_ORANGE));
    lv_style_set_text_font(&Time, &Builder);
  
    label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "11:11");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_add_style(label, &Time, 0);
  
    // delay(2000);
  

    lv_style_init(&DP_IMG);
    lv_style_set_pad_all(&DP_IMG, 0);
    lv_style_set_margin_bottom(&DP_IMG, 0);
    lv_style_set_margin_top(&DP_IMG, 0);
    lv_style_set_margin_left(&DP_IMG, 0);
    lv_style_set_margin_right(&DP_IMG, 0);
  
    LV_IMAGE_DECLARE(dp1);
    img1 = lv_image_create(lv_screen_active());
    lv_image_set_src(img1, &dp1);
    lv_obj_align(img1, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_scrollbar_mode(lv_screen_active(), LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(img1, &DP_IMG, 0);
}

void updateTime()
{
    char t[10] ={'\0'};
    ct+=1;
    sprintf(t, "%d:%d",11,ct);
    lv_label_set_text(label, t);
    Serial.printf("New time: %s\n",t);
}