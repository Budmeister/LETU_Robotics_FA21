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

#define A_PIN 4
#define B_PIN 5
#define X_PIN 6
#define Y_PIN 7
#define R_PIN 8
#define LX_PIN 9
#define LY_PIN 10

void setup(){
    Serial.begin(9600);
    Mirf.cePin = 7;
    Mirf.csnPin = 8;

    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();

    Mirf.setTADDR((byte*) MAIN_ADDR);

    Mirf.payload = RF_PAYLOAD;
    Mirf.channel = 10;
    Mirf.config();

    pinMode(A_PIN, OUTPUT);
    pinMode(B_PIN, OUTPUT);
    pinMode(X_PIN, OUTPUT);
    pinMode(Y_PIN, OUTPUT);
    pinMode(R_PIN, OUTPUT);
    pinMode(LX_PIN, OUTPUT);
    pinMode(LY_PIN, OUTPUT);

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
char* getData(){
    char a = digitalRead(A_PIN);
    char b = digitalRead(B_PIN);
    char x = digitalRead(X_PIN);
    char y = digitalRead(Y_PIN);
    char r = digitalRead(R_PIN);
    int lx = analogRead(LX_PIN);
    int ly = analogRead(LY_PIN);

    char data[RF_PAYLOAD];
    data[RF_PAYLOAD - 1] = 
            (a << A_CONTROL) |
            (b << B_CONTROL) |
            (x << X_CONTROL) |
            (y << Y_CONTROL) |
            (r << R_CONTROL);
    data[RF_PAYLOAD - 2] = ly & 0x00ff;
    data[RF_PAYLOAD - 3] = ly & 0xff00;
    data[RF_PAYLOAD - 4] = lx & 0x00ff;
    data[RF_PAYLOAD - 5] = lx & 0xff00;
    for(int i = 0; i < RF_PAYLOAD - 5; i++)
        data[i] = 0;
    return (char*) data;
}

void loop(){
    delay(1000);
    Serial.println("hello_world");
    
    Mirf.send(getData());
}