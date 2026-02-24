/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/integration/framework/arduino.html  */

#include <Arduino.h>
#include <lvgl.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoTrace.h>
// #if LV_USE_TFT_ESPI
// #include <TFT_eSPI.h>
// #endif

// #include "dp1.h"
// #include "Builder.h"

#include "Display.h"
#include "IMU.h"
#include "Touch.h"
#include "UI/UI.h"
// #include "WatchFace.h"

Adafruit_NeoPixel strip(1, 21, NEO_RGB + NEO_KHZ800);

void setup()
{
  // String LVGL_Arduino = "Hello Arduino! ";
  // LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  // delay(200);
  Serial.begin(115200);
  strip.begin();
  strip.setBrightness(15);
  strip.show();
  // delay(5000);
  Serial.println("Starting Up...");
  // TRACE();
  // Serial.println(LVGL_Arduino);

  setupTouch();
  // TRACE();

  setupDisplay();

  // TRACE();

  delay(1000);

  drawUI();

  Serial.println("Setup done");

  strip.setPixelColor(0, 0, 255, 0);
  strip.show();

  startLVGLTask();
}

void loop()
{
  // lv_timer_handler(); /* let the GUI do its work */
  // vTaskDelay(5000); /* let this time pass */
  // updateTime();
}