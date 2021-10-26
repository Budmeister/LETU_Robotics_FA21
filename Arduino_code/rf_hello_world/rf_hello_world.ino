#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
// #include "global_vars.h"
#define MAIN_ADDR "main_"
#define RF_PAYLOAD 4

void setup(){
    Serial.begin(9600);

    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();

    Mirf.setTADDR((char*) "Rec01");

    Mirf.payload = RF_PAYLOAD;
    Mirf.channel = 3;
    Mirf.config();

    Serial.println("Setup complete");

}

void loop(){
    // char data[RF_PAYLOAD];
//    Mirf.dataReady();
//    if(Mirf.isSending())
//        return;
//    char* data = "Hello..\n";
    Serial.println("Sending data");
    char data[] = {
        0,1,2,3
    };
    Mirf.send(data);
    Serial.print("Sent: ");
    for(int i = 0; i < RF_PAYLOAD; i++)
        Serial.print((int) data[i]);
    Serial.println();
}
