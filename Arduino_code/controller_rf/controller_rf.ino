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
#define A_CONTROL 5
#define B_CONTROL 4
#define X_CONTROL 3
#define Y_CONTROL 2
#define LB_CONTROL 1
#define RB_CONTROL 0
#define JX_CONTROL 16
#define JY_CONTROL 32

#define A_PIN 7
#define B_PIN 6
#define X_PIN 5
#define Y_PIN 4
#define LB_PIN 3
#define RB_PIN 2
#define JX_PIN A0
#define JY_PIN A1

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
    pinMode(LB_PIN, INPUT_PULLUP);
    pinMode(RB_PIN, INPUT_PULLUP);
    pinMode(JX_PIN, INPUT);
    pinMode(JY_PIN, INPUT);

    Serial.println("Setup complete");

}

/*
Returns the 8 byte data array:
0: 3 bytes
LX: 2 bytes
LY: 2 bytes
Btns: 1 byte

Btns: 
0: 2 bits
A: 1 bit
B: 1 bit
X: 1 bit
Y: 1 bit
L: 1 bit
R: 1 bit
*/
uint8_t a, b, x, y, lb, rb;
int jx, jy;

void printData(){
    Serial.print(jx);
    Serial.print(" ");
    Serial.print(jy);
    Serial.print(" ");
    Serial.print(a);
    Serial.print(b);
    Serial.print(x);
    Serial.print(y);
    Serial.print(lb);
    Serial.print(rb);
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
    lb = digitalRead(LB_PIN);
    rb = digitalRead(RB_PIN);
    jx = analogRead(JX_PIN);
    jy = analogRead(JY_PIN);

    uint8_t data[RF_PAYLOAD];
    data[RF_PAYLOAD - 1] = 
            (a << A_CONTROL) |
            (b << B_CONTROL) |
            (x << X_CONTROL) |
            (y << Y_CONTROL) |
            (lb << LB_CONTROL) |
            (rb << RB_CONTROL);
    data[RF_PAYLOAD - 2] = jy & 0x00ff;
    data[RF_PAYLOAD - 3] = (jy & 0xff00) >> 8;
    data[RF_PAYLOAD - 4] = jx & 0x00ff;
    data[RF_PAYLOAD - 5] = (jx & 0xff00) >> 8;
    for(int i = 0; i < RF_PAYLOAD - 5; i++)
        data[i] = 0;

    Mirf.send(data);
}