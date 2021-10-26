#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
// #include "global_vars.h"
#define MAIN_ADDR "main_"
#define RF_PAYLOAD 8

/*
Controls:
A, B, X, Y, R: 1 bit ea
LX, LY: 1 byte ea
*/
#define A_CONTROL 4
#define B_CONTROL 3
#define X_CONTROL 2
#define Y_CONTROL 1
#define R_CONTROL 0
#define LX_CONTROL 16
#define LY_CONTROL 32

#define A_PIN 6
#define B_PIN 5
#define X_PIN 4
#define Y_PIN 3
#define R_PIN 2
#define LX_PIN A0
#define LY_PIN A1

void setup(){
    Serial.begin(9600);
    Mirf.cePin = 9;
    Mirf.csnPin = 10;

    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();

    Mirf.setRADDR((byte*) MAIN_ADDR);

    Mirf.payload = RF_PAYLOAD;
    Mirf.channel = 3;
    Mirf.config();

    Serial.println("Setup complete");

}

/*
Returns the 8 byte data array:
0: 3 bytes
LX: 2 bytes
LY: 2 bytes
Btns: 1 byte

Btns: 
0: 3 bits
A: 1 bit
B: 1 bit
X: 1 bit
Y: 1 bit
R: 1 bit
*/
byte a, b, x, y, r;
int lx, ly;

void unpackData(){
    byte data[RF_PAYLOAD];
    Mirf.getData(data);

    ly = (unsigned int) (data[RF_PAYLOAD - 2]
            | (data[RF_PAYLOAD - 3] << 8));
    lx = (unsigned int) (data[RF_PAYLOAD - 4]
            | (data[RF_PAYLOAD - 5] << 8));
    a = (data[RF_PAYLOAD - 1] >> A_CONTROL) & 0x1;
    b = (data[RF_PAYLOAD - 1] >> B_CONTROL) & 0x1;
    x = (data[RF_PAYLOAD - 1] >> X_CONTROL) & 0x1;
    y = (data[RF_PAYLOAD - 1] >> Y_CONTROL) & 0x1;
    r = (data[RF_PAYLOAD - 1] >> R_CONTROL) & 0x1;

}

void printData(){
    Serial.print(lx);
    Serial.print(" ");
    Serial.print(ly);
    Serial.print(" ");
    Serial.print(a);
    Serial.print(b);
    Serial.print(x);
    Serial.print(y);
    Serial.print(r);
    Serial.println();
}

void loop(){
    if(Mirf.dataReady()){
        unpackData();
        printData();
    }
}