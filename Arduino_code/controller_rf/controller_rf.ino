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

    Mirf.setTADDR((byte*) MAIN_ADDR);

    Mirf.payload = RF_PAYLOAD;
    Mirf.channel = 3;
    Mirf.config();

    pinMode(A_PIN, INPUT_PULLUP);
    pinMode(B_PIN, INPUT_PULLUP);
    pinMode(X_PIN, INPUT_PULLUP);
    pinMode(Y_PIN, INPUT_PULLUP);
    pinMode(R_PIN, INPUT_PULLUP);
    pinMode(LX_PIN, INPUT);
    pinMode(LY_PIN, INPUT);

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
uint8_t a, b, x, y, r;
int lx, ly;

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

void printData(uint8_t* data){
    for(int i = 0; i < RF_PAYLOAD; i++){
        Serial.print(data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

void loop(){
    while(Mirf.isSending());
    
    // pack data
    a = digitalRead(A_PIN);
    b = digitalRead(B_PIN);
    x = digitalRead(X_PIN);
    y = digitalRead(Y_PIN);
    r = digitalRead(R_PIN);
    lx = analogRead(LX_PIN);
    ly = analogRead(LY_PIN);

    uint8_t data[RF_PAYLOAD];
    data[RF_PAYLOAD - 1] = 
            (a << A_CONTROL) |
            (b << B_CONTROL) |
            (x << X_CONTROL) |
            (y << Y_CONTROL) |
            (r << R_CONTROL);
    data[RF_PAYLOAD - 2] = ly & 0x00ff;
    data[RF_PAYLOAD - 3] = (ly & 0xff00) >> 8;
    data[RF_PAYLOAD - 4] = lx & 0x00ff;
    data[RF_PAYLOAD - 5] = (lx & 0xff00) >> 8;
    for(int i = 0; i < RF_PAYLOAD - 5; i++)
        data[i] = 0;

    Mirf.send(data);
}