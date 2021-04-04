/*=========================================================================
*        Kumandadan okunan deger alıcıya gonderilir.
*        1. nolu modul
*   
 =========================================================================*/

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <ESP8266WiFi.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <espnow.h>

/*=========================================================================  
 *          Alıcının MAC Adresini buraya yazalım 2 numaralı modul.  
 =========================================================================*/  
uint8_t broadcastAddress[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  

/*=========================================================================  
 *         gönderilecek veriler için bir veri yapısı tanımlamamız lazım
*          Bu veri yapısı alıcı tarafında da aynı olmalı
 =========================================================================*/  
typedef struct struct_message
{
  int id;               // kart numarası
  int code;             // kumanda kodu.
} struct_message;
/*=========================================================================  
 *    tanımladığımız veri yapısı türünde myData isimli bir veri oluşturduk  
 =========================================================================*/  
struct_message kumanda;

/*=========================================================================  
 *           SCL-> Pinine baglı. 2 nolu pin SDA->0 nolu pindir
 =========================================================================*/  
const uint16_t kRecvPin = 2;  
IRrecv irrecv(kRecvPin);
decode_results results;

/*======================================================================
 *           Veri gönderme yapıldığında bu fonk çalışacak
=======================================================================*/
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
  Serial.print("Son mesaj paketinin gönderim durumu: ");
  if (sendStatus == 0)
                      {
                        Serial.println("Gönderme Başarılı");
                      }
                      else
                          {
                            Serial.println("Gönderme Hatası");
                          }
}
/*======================================================================
 *           SETUP
=======================================================================*/
void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);      // cihazı WIFI STATION olarak çalıştırmamız gerekiyor 
  if (esp_now_init() != 0)  // ESP-NOW özelliğini başlatıyoruz
          {
            Serial.println("ESP-NOW Başlatılamadı!!!");
            return;
          }
/*==========================================================================
*         Kartın rolünü tanımlıyoruz
*         send cb fonksiyonu ile de veri gittiğinde onDataSent 
*         fonksiyonunun çağrılmasını sağlıyoruz.
 ==========================================================================*/
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
/*=========================================================================  
 *            yukarıdaki mac adresli alıcıyı eşleştiriyoruz.
 =========================================================================*/  
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  irrecv.enableIRIn();    // Start the receiver
  while (!Serial)         // Wait for the serial connection to be establised.
    delay(50);
  Serial.println();
  Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
  Serial.println(kRecvPin);
  kumanda.id =1;
}
/*=========================================================================  
 *            LOOP
 =========================================================================*/  
void loop()
{
  if (irrecv.decode(&results))
  {
    Serial.print((uint32_t)results.value, HEX);
    kumanda.code =(uint32_t)results.value;
    esp_now_send(broadcastAddress, (uint8_t *) &kumanda, sizeof(kumanda));
    Serial.println("");
    irrecv.resume();  // Receive the next value 
  }
 // delay(100);  
 // esp_now_send(broadcastAddress, (uint8_t *) &kumanda, sizeof(kumanda));  
}
/*=========================================================================  
 *           
 =========================================================================*/  
