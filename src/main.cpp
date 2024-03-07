// Bibliotecas Destinadas ao ESP32
#include <Arduino.h>
#include <WiFi.h>

// Bibliotecas Display ESP32
#include <esp32_smartdisplay.h>
#include <ui/ui.h>

// Declarações iniciais
void wifiServerSetup();
void wifiServerRun();
#define WIFI_SSID "LIVE TIM_EFB8_2G"
#define WIFI_PASSWORD "wlan541047"
bool ipuser = false;
WiFiServer server(80);



void setup()
{
  // Serial Monitor SID = 115200
  Serial.begin(115200);

// Verifica se conectado ao USB
#ifdef ARDUINO_USB_CDC_ON_BOOT
  delay(1000);
#endif


  // Inicia a Biblioteca do display
  smartdisplay_init();

  //Orientação dos Dispositivo
  // auto disp = lv_disp_get_default();
  // lv_disp_set_rotation(disp, LV_DISP_ROT_90);
  // lv_disp_set_rotation(disp, LV_DISP_ROT_180);
  // lv_disp_set_rotation(disp, LV_DISP_ROT_270);

  // Inicia o display
  ui_init();

  wifiServerSetup();

  pinMode(GPIO_NUM_22, OUTPUT);

  digitalWrite(GPIO_NUM_22, HIGH);
  delay(1000);
  digitalWrite(GPIO_NUM_22, LOW);
  
}

void loop()
{

  auto const now = millis();

  #ifdef BOARD_HAS_RGB_LED
        auto const rgb = (now / 2000) % 8;
        smartdisplay_led_set_rgb(rgb & 0x01, rgb & 0x02, rgb & 0x04);
  #endif
  #ifdef BOARD
  #endif
  
  wifiServerRun();

  lv_timer_handler();
}

void setIPaddress(lv_event_t *e)
{
  Serial.println(WiFi.localIP());

  String ipAddress = WiFi.localIP().toString();

  // Converta a string para um array de caracteres (const char *)
  const char *ip = ipAddress.c_str();

  // Defina o texto do rótulo com o endereço IP
  lv_label_set_text(ui_ipAddress, ip);
}

void userPaymentSuccesful(lv_event_t *e)
{
  
  ipuser = true;
  Serial.println("ipuser: ");
  Serial.println(ipuser);
}



void wifiServerSetup(){
  // Wifi Test 
  
  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_MODE_APSTA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectada.");
  Serial.println("Endereço de IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
// end Wifi Test
}



void wifiServerRun(){
  if (ipuser)
  {
    WiFiClient client = server.available();
    if (client)
    {                               // if you get a client,
      Serial.println("new client"); // print a message out the serial port
      String currentLine = "";      // make a String to hold incoming data from the client
      while (client.connected())
      { // loop while the client's connected
        if (client.available())
        {                         // if there's bytes to read from the client,
          char c = client.read(); // read a byte, then
          Serial.write(c);        // print it out the serial monitor
          if (c == '\n')
          { // if the byte is a newline character

            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0)
            {
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();

              // the content of the HTTP response follows the header:
              client.println("<head><style>h1 { color: green; text-align: center; }</style></head>");
              client.println("<h1>Pagamento Concluido</h1>");
              digitalWrite(GPIO_NUM_22, HIGH);

              // The HTTP response ends with another blank line:
              client.println();

              // break out of the while loop:
              break;
            }
            else
            { // if you got a newline, then clear currentLine:
              currentLine = "";
            }
          }
          else if (c != '\r')
          {                   // if you got anything else but a carriage return character,
            currentLine += c; // add it to the end of the currentLine
          }
        }
      }
      delay(8000);
      digitalWrite(GPIO_NUM_22, LOW);
      client.stop();
      Serial.println("client disonnected");
    }
  }
}