// Bibliotecas Destinadas ao ESP32
#include <Arduino.h>
#include <WiFi.h>

// Bibliotecas Display ESP32
#include <esp32_smartdisplay.h>
#include <ui/ui.h>

// Declarações iniciais
#define WIFI_SSID "LIVE TIM_EFB8_2G"
#define WIFI_PASSWORD "wlan541047"
bool ipuser = false;
WiFiServer server(80);



void setup() {
  // Serial Monitor SID = 115200
  Serial.begin(115200);

  //Wifi Test
  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_MODE_APSTA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectada.");
  Serial.println("Endereço de IP: ");
  Serial.println(WiFi.localIP());
  
  server.begin();

  // end Wifi Test

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

  

  // auto const now = millis();
  // #ifdef BOARD_HAS_RGB_LED
  //       auto const rgb = (now / 2000) % 8;
  //       smartdisplay_led_set_rgb(rgb & 0x01, rgb & 0x02, rgb & 0x04);
  // #endif

  if (ipuser){
   WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print("Pagamento Concluido");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
  } 
  }
  lv_timer_handler();
}


void setIPaddress(lv_event_t *e){
  Serial.println(WiFi.localIP());

  String ipAddress = WiFi.localIP().toString();

  // Converta a string para um array de caracteres (const char *)
  const char *ip = ipAddress.c_str();

  // Defina o texto do rótulo com o endereço IP
  lv_label_set_text(ui_ipadress, ip);
}

void userPaymentSuccesful(lv_event_t *e){
  ipuser = true;
}