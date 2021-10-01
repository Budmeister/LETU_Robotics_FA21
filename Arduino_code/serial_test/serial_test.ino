#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
// #include "global_vars.h"
#define MAIN_ADDR "main_"
#define RF_PAYLOAD 8

void setup(){
    Serial.begin(9600);
    Mirf.cePin = 7;
    Mirf.csnPin = 8;

    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();

    Mirf.setRADDR((byte*) MAIN_ADDR);

    Mirf.payload = RF_PAYLOAD;
    Mirf.channel = 10;
    Mirf.config();

    Serial.println("Setup complete");

}

void loop(){
    delay(1000);
    Serial.println("hello_world");
    char data[RF_PAYLOAD];
    Mirf.getData((char*) data);
    Serial.println((char*) data);
}