#include<ESP8266WiFi.h>  //Using library for ESP8266 WiFi
#include<espnow.h>    //Using Library for ESP-NOW Protocol


uint8_t broadcastAddress1[] = {0xE8, 0xDB, 0x84, 0xAF, 0x6C, 0x5E};   // Mac. Address of BOT1 ESP8266 
uint8_t broadcastAddress2[] = {0xE8, 0xDB, 0x84, 0xAE, 0x1F, 0x4F};   // Mac. Address of BOT2 ESP8266 
uint8_t broadcastAddress3[] = {0xE8, 0xDB, 0x84, 0xAD, 0x97, 0xB5};   // Mac. Address of BOT3 ESP8266 
uint8_t broadcastAddress4[] = {0x50, 0x02, 0x91, 0x5F, 0xE2, 0xDE};   // Mac. Address of BOT4 ESP8266 

typedef struct server {
    String dir;
    String ang;
    String servo;
} server;

int dataLength = 4;
String s[4]; 

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  char macStr[18];
  Serial.print("Packet to:");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
         mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}


void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

   // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  // Register peer
  esp_now_add_peer(broadcastAddress1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress2, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress3, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress4, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
 int j=0;
 while(Serial.available())
 {
  char ch = Serial.read();
  if(ch==',')
  {
    j++;
  }
  else
  s[j]+=ch;
 }

server DATA = {s[1],s[2],s[3]};
delay(100);

if(s[0] == "bot1")
{
  esp_now_send(broadcastAddress1, (uint8_t *) &DATA, sizeof(DATA)); 
}
else if(s[0] == "bot2")
{
  esp_now_send(broadcastAddress2, (uint8_t *) &DATA, sizeof(DATA)); 
}
else if(s[0] == "bot3")
{
  esp_now_send(broadcastAddress3, (uint8_t *) &DATA, sizeof(DATA)); 
}
else if(s[0] == "bot4")
{
  esp_now_send(broadcastAddress4, (uint8_t *) &DATA, sizeof(DATA)); 
}

for(int i=0;i<4;i++)
{
  s[i] = "";
}

}
