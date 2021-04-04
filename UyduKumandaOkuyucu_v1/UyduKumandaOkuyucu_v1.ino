/*=========================================================================
*        Alıcı modul. Kumandadan gelen veriler IR ile gonderilir.
*        2. nolu modul 
*   Veri almasi tamamdır.
 =========================================================================*/

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <ESP8266WiFi.h>
#include <IRsend.h>
#include <IRutils.h>
#include <espnow.h>


#define IR_SEND_PIN 3  // SDA pin ->0 SCL->2 
IRsend irsend(IR_SEND_PIN);

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
 *    VERİ ALIMI OLDUĞUNDA BU FONKSİYON ÇALIŞACAK  
 =========================================================================*/
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len)
{
  memcpy(&kumanda, incomingData, sizeof(kumanda));
/*
<  Serial.print("Byte veri alındı: ");
  Serial.println(len);
  Serial.print("Kart ID: ");
  Serial.println(kumanda.id);
  Serial.print("Kumanda Kodu: ");
  Serial.println(kumanda.code,HEX);
  */
  Serial.println();  
}

/*=========================================================================  
 *           SCL-> Pinine baglı. 2 nolu pin SDA->0 nolu pindir
 =========================================================================*/  
//const uint16_t kRecvPin = 2;  
//IRrecv irrecv(kRecvPin);
decode_results results;

/*======================================================================
 *           SETUP
=======================================================================*/
void setup()
{
  Serial.begin(115200);
  irsend.begin();
  WiFi.mode(WIFI_STA);      // cihazı WIFI STATION olarak çalıştırmamız gerekiyor 
  if (esp_now_init() != 0)  // ESP-NOW özelliğini başlatıyoruz
          {
            Serial.println("ESP-NOW Başlatılamadı!!!");
            return;
          }
/*==========================================================================
*         ESPNOW başlatıldığında  
*         kartın rolünü tanımladık ve veri alındığında çağrılacak 
*         fonksiyonu ilişkilendiriyoruz.
 ==========================================================================*/
   esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
   esp_now_register_recv_cb(OnDataRecv); 
}
/*=========================================================================  
 *            LOOP
 =========================================================================*/  
void loop()
{
  if(kumanda.code>0){
     irsend.sendNEC(kumanda.code, 32);
    // delay(100); 
     kumanda.code = 0;
  } 
 // delay(100); 
}
/*=========================================================================  
 *           
 =========================================================================*/  
