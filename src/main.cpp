// Bibliotecas Destinadas ao ESP32
#include <Arduino.h>
#include <WiFi.h>

// Bibliotecas Display ESP32
#include <esp32_smartdisplay.h>
#include <ui/ui.h>

// Declarações iniciais
const char* ssid = "LIVE TIM_EFB8_5G";
const char* password = "wlan541047";
WiFiServer server(80);


void setup() {
  // Serial Monitor SID = 115200
  Serial.begin(115200);

  // Verifica se conectado ao USB
  #ifdef ARDUINO_USB_CDC_ON_BOOT    
    delay(1000);
  #endif

  // Inicia a Biblioteca do display
  smartdisplay_init();

  auto disp = lv_disp_get_default();
  //lv_disp_set_rotation(disp, LV_DISP_ROT_90);
  // lv_disp_set_rotation(disp, LV_DISP_ROT_180);
  //lv_disp_set_rotation(disp, LV_DISP_ROT_270);


  // Inicia o display
  ui_init();
}

void loop() {

  lv_timer_handler();
}

