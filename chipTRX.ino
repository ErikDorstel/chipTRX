#include "DRA818V.h"

DRA818V trx;

void doTone(int cycles,int duration) {
  int freq=200; for (int cycle=0;cycle<cycles;cycle++) {
    tone(analogOut,freq); delay(duration); noTone(analogOut);
    freq+=200; if (freq>2000) { freq=200; } } }

void setup() {
  trx.begin(144.5,144.5);
  trx.setPTT(true,false);
  doTone(50,100);
  trx.setPTT(false); }

void loop() {}
