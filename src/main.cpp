#include <Arduino.h>
#include <LoRa.h>

#define LORA_NSS_PIN 5
#define LORA_RST_PIN 22
#define LORA_DIO0_PIN 21

void lora_init() {
    const long FREQ = 433.800e6; // channel 30
    
    LoRa.setPins(LORA_NSS_PIN, LORA_RST_PIN, LORA_DIO0_PIN);

    if(!LoRa.begin(FREQ)) {
        for(;;)   
            Serial.println("[!] LoRa begin() failed!");
        delay(1000);
    }

    // Calculated for ~500 bps (sf11) or ~900 bps (sf10) 
    LoRa.setCodingRate4(5);
    LoRa.setSpreadingFactor(10);
    LoRa.setSignalBandwidth(125000);
    
    LoRa.setTxPower(0); // MAX POWER (20 dBm - 100mW)
}

void setup() {
    Serial.begin(115200);
    lora_init();
}

const int FRAME_SIZE = 50;
char data[FRAME_SIZE];
char hexdata[2*FRAME_SIZE];
void loop() {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {

        // read packet
        int len = 0;
        while (LoRa.available()) {
            unsigned char byte = LoRa.read();
            data[len++] = byte;
        }

        for(int i=0; i<len; i++) {
            char hexz[4];
            sprintf(hexz, "%02x", (unsigned char)data[i]);
            hexdata[i*2] = hexz[0];
            hexdata[i*2+1] = hexz[1];
        }
        hexdata[2*len] = '0';
        hexdata[2*len+1] = '0';

        /*
        // print RSSI of packet
        Serial.print("rssi=");
        Serial.print(LoRa.packetRssi());
        Serial.print(" len=");
        Serial.print(len);
        Serial.print(" data=");
        Serial.print(data);
        Serial.print(" hexdata=");
        Serial.println(hexdata);
        */
       
        Serial.print(LoRa.packetRssi());
        Serial.print(" ");
        Serial.println(hexdata);
    }
}