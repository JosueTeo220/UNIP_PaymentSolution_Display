#include <Arduino.h>

#include <esp32_smartdisplay.h>
#include <ui/ui.h>



void setup() {

  Serial.begin(115200);

  #ifdef ARDUINO_USB_CDC_ON_BOOT    
    delay(1000);
  #endif


  smartdisplay_init();
  auto disp = lv_disp_get_default();
  //lv_disp_set_rotation(disp, LV_DISP_ROT_90);
  // lv_disp_set_rotation(disp, LV_DISP_ROT_180);
  //lv_disp_set_rotation(disp, LV_DISP_ROT_270);
  ui_init();

}

void loop() {

  lv_timer_handler();
  
}

