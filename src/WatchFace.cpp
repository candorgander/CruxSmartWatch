#include <Arduino.h>
#include <ArduinoTrace.h>
#include <lvgl.h>

// #include "Builder.h"
#include "dp1.h"
#include "Builder.h"
#include "SettingsPage.h"

#include <Touch.h>
#include "Display.h"
#include "RTOS.h"

static lv_style_t main_screen;
static lv_style_t Time;
lv_obj_t *label;
static lv_style_t DP_IMG;
lv_obj_t *img1;
int ct = 11;

static lv_obj_t *list1;

static lv_key_t lastKey = LV_KEY_NEXT;

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
    char t[10] = {'\0'};
    ct += 1;
    sprintf(t, "%d:%d", 11, ct);
    lv_label_set_text(label, t);
    Serial.printf("New time: %s\n", t);
}

void showSettingsPage()
{
    LV_IMAGE_DECLARE(Settings_Page);
    img1 = lv_image_create(lv_screen_active());
    lv_image_set_src(img1, &Settings_Page);
    lv_obj_align(img1, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_scrollbar_mode(lv_screen_active(), LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(img1, &DP_IMG, 0);
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

static void menu_item_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    const char *label = (const char *)lv_event_get_user_data(e);

    if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);
        Serial.printf("[Menu] key=%u on item '%s'\n", key, label);
        if (key == LV_KEY_ENTER)
        {
            Serial.printf("[Menu] Opening: %s\n", label);
            //   create_detail_screen(label);
        }
    }
    else if (code == LV_EVENT_FOCUSED)
    {
        Serial.printf("[Menu] Focused: %s\n", label);
    }
    else if (code == LV_EVENT_CLICKED)
    {
        Serial.printf("[Menu] Clicked: %s\n", label);
        // create_detail_screen(label);
    }
}

static const char *menu_items[] = {
    "Watch Face",
    "Steps",
    "Sleep",
    "Settings"};
static const uint32_t MENU_COUNT = sizeof(menu_items) / sizeof(menu_items[0]);

void buttonTest()
{
    takeLVGLMutex();
    TRACE();
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_KEYPAD);
    lv_indev_set_read_cb(indev, touchcb);
    lv_indev_set_disp(indev, disp);
    // lv_indev_add_event_cb(indev, key_indev_cb, LV_EVENT_ALL, NULL);

    static lv_group_t *group = lv_group_create();
    lv_group_set_default(group);
    lv_indev_set_group(indev, group);

    static lv_obj_t *main_screen = lv_screen_active();
    lv_obj_set_style_bg_color(main_screen, lv_color_hex(0x0d0d1a), 0);
    lv_obj_set_style_pad_all(main_screen, 0, 0);

    // Header
    lv_obj_t *header = lv_obj_create(main_screen);
    lv_obj_set_size(header, LV_PCT(100), 40);
    lv_obj_set_style_bg_color(header, lv_color_hex(0x16213e), 0);
    lv_obj_set_style_border_width(header, 0, 0);
    lv_obj_set_style_radius(header, 0, 0);
    lv_obj_align(header, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t *title = lv_label_create(header);
    lv_label_set_text(title, "MENU");
    lv_obj_set_style_text_color(title, lv_color_hex(0xe94560), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_center(title);

    // Scrollable list container
    lv_obj_t *list = lv_obj_create(main_screen);
    lv_obj_set_size(list, LV_PCT(100), LV_PCT(100) - 40);
    lv_obj_align(list, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_flex_flow(list, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(list, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(list, lv_color_hex(0x0d0d1a), 0);
    lv_obj_set_style_border_width(list, 0, 0);
    lv_obj_set_style_pad_row(list, 6, 0);
    lv_obj_set_style_pad_all(list, 8, 0);
    lv_obj_set_scroll_dir(list, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(list, LV_SCROLLBAR_MODE_ACTIVE);

    for (uint32_t i = 0; i < MENU_COUNT; i++)
    {
        lv_obj_t *btn = lv_button_create(list);
        lv_obj_set_size(btn, LV_PCT(100), 44);
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x16213e), 0);
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x0f3460), LV_STATE_FOCUSED);
        lv_obj_set_style_border_color(btn, lv_color_hex(0xe94560), LV_STATE_FOCUSED);
        lv_obj_set_style_border_width(btn, 2, LV_STATE_FOCUSED);
        lv_obj_set_style_border_width(btn, 0, 0);
        lv_obj_set_style_radius(btn, 8, 0);

        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, menu_items[i]);
        lv_obj_set_style_text_color(lbl, lv_color_hex(0xe0e0e0), 0);
        lv_obj_set_style_text_color(lbl, lv_color_hex(0xffffff), LV_STATE_FOCUSED);
        lv_obj_align(lbl, LV_ALIGN_LEFT_MID, 12, 0);

        // Arrow indicator
        lv_obj_t *arrow = lv_label_create(btn);
        lv_label_set_text(arrow, LV_SYMBOL_RIGHT);
        lv_obj_set_style_text_color(arrow, lv_color_hex(0xe94560), 0);
        lv_obj_align(arrow, LV_ALIGN_RIGHT_MID, -12, 0);

        lv_obj_add_event_cb(btn, menu_item_cb, LV_EVENT_KEY, (void *)menu_items[i]);
        lv_obj_add_event_cb(btn, menu_item_cb, LV_EVENT_FOCUSED, (void *)menu_items[i]);
        lv_obj_add_event_cb(btn, menu_item_cb, LV_EVENT_CLICKED, (void *)menu_items[i]);

        lv_group_add_obj(group, btn);
    }

    // Focus first item
    lv_group_focus_next(group);
    relaseLVGLMutex();

    // lv_screen_load(main_screen);
}

void buttontest2()
{
    takeLVGLMutex();
    TRACE();
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_KEYPAD);
    lv_indev_set_read_cb(indev, touchcb);
    lv_indev_set_disp(indev, disp);

    static lv_group_t *group = lv_group_create();
    lv_group_set_default(group);
    lv_indev_set_group(indev, group);

    list1 = lv_list_create(lv_screen_active());
    lv_obj_center(list1);
    lv_obj_t *btn;
    lv_list_add_text(list1, "File");
    btn = lv_list_add_button(list1, LV_SYMBOL_FILE, "New");
    lv_obj_add_event_cb(btn, menu_item_cb, LV_EVENT_KEY, (void *)"New");
    lv_obj_add_event_cb(btn, menu_item_cb, LV_EVENT_FOCUSED, (void *)"New");
    lv_group_add_obj(group, btn);
    btn = lv_list_add_button(list1, LV_SYMBOL_DIRECTORY, "Open");
    lv_obj_add_event_cb(btn, menu_item_cb, LV_EVENT_KEY, (void *)"Open");
    lv_obj_add_event_cb(btn, menu_item_cb, LV_EVENT_FOCUSED, (void *)"Open");
    lv_group_add_obj(group, btn);

    lv_group_add_obj(group, list1);
    lv_group_focus_next(group);

    relaseLVGLMutex();
}