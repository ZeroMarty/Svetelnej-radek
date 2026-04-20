#include <avr/io.h>
#include <util/delay.h>
#include "font8x8_basic.h"
//MAX7219
//SPI končí až kdyź končí, Ovládej v tomto pořadí: Shutdown -> Decode -> Scan limit -> Intenzita -> Display test
#define DIN 0x01 //PORTB0
#define CS 0x02 //PORTB1
#define CLK 0x04 // PORTB2
#define SPI_DELAY 50
#define pocet 12
#define pocet_sloupcu (pocet * 8)
int hodiny = 14;
int minuty = 25;
int sekundy = 30;
int den = 21;
int mesic = 4;
int rok = 2026;

void setup() {
  DDRB = 0b11111111;
  PORTB = 0b00000010;
}

void spi_start() {
  PORTB &= ~(CS);
  _delay_us(SPI_DELAY);
}
void spi_stop() {
  PORTB |= CS;
  _delay_us(SPI_DELAY);
}

void pis_vsem(uint8_t addr, uint8_t data) {
  spi_start();
  for (int i = 0; i < pocet; i++) { //pošle na každý registr
    spi_pis(addr, data);
  }
  spi_stop();
}

void pis_jednomu(uint8_t displej, uint8_t addr, uint8_t data) { //za pomocí noop napíše data jen a pouze pro námi zvolený displej
  spi_start();
  for(int i = pocet - 1; i> displej, i--) {
    spi_pis(0x00, 0x00);
  }
  spi_pis(addr, data);
  for(int i = displej -1; i>=0; i--) {
    spi_pis (0x00, 0x00);
  }
  spi_stop();
}

void spi_pis(uint8_t addr, uint8_t data) {
  for(int i = 0; i < 8; i++) {
    if((addr & 0x8000) == 0) {
      PORTB &= ~(DIN);
      _delay_us(SPI_DELAY);
    }
    else {
      PORTB |= DIN;
      _delay_us(SPI_DELAY);
    }
    PORTB |= CLK;
    _delay_us(SPI_DELAY);
    PORTB &= ~(CLK);
    _delay_us(SPI_DELAY);
    addr = addr << 1;
  }
  for(int i = 0; i < 8; i++) {
    if((data & 0x8000) == 0) {
      PORTB &= ~(DIN);
      _delay_us(SPI_DELAY);
    }
    else {
      PORTB |= DIN;
      _delay_us(SPI_DELAY);
    }
    PORTB |= CLK;
    _delay_us(SPI_DELAY);
    PORTB &= ~(CLK);
    _delay_us(SPI_DELAY);
    data = data << 1;
  }
}

void scan() {
  pis_vsem(0x0B, 0x07);
}

void smaz() {
  
}
/*void test() { adresy 01 - 08 jsou pozice
  spi_pis(0x01, 0x01);
  spi_pis(0x02, 0x00);
  spi_pis(0x03, 0x00);
  spi_pis(0x04, 0x00);
  spi_pis(0x05, 0x00);
  spi_pis(0x06, 0x00);
  spi_pis(0x07, 0x00);
  spi_pis(0x08, 0x00);
}*/ 

void spi_vypni() {
  pis_vsem(0x0C, 0x00);
}

void spi_zapni() {
  pis_vsem(0x0C, 0x01);
}

void intenzita() {
  pis_vsem(0x0A, 0x0F);
}

void decode() {
  pis_vsem(0x09, 0x00);
}

void display_test() {
  pis_vsem(0x0F, 0x00);
}
void setup_spi() {
  spi_vypni();
  spi_zapni();
  decode();
  scan();
  intenzita();
  display_test();
}

void prehoz(int *a, int *b) {
  *a = 0;
  (*b)++;
}

void update_cas() {
  sekundy++;
  if(sekundy == 61) {
    prehoz(&sekundy, &minuty);
  }
  if(minuty == 61) {
    prehoz(&minuty, &hodiny);
  }
  if(hodiny == 25) {
    prehoz(&hodiny, &den);
  }
  if(den == 31) {
    prehoz(&den, &mesic);
  }
  if(mesic == 13) {
    prehoz(&mesic, &rok);
  }
} 

int main() {
  bool cas = true; //datum/čas přehoz
  int pocitadlo = 0;
  setup();
  setup_spi();  
  while(1) {
    while(cas == true) {
      update_cas();

      _delay_ms(1000);
      pocitadlo++;
      if(pocitadlo == 10) {
        pocitadlo = 0;
        cas = false;
      }
    }
    while(cas == false) {
      update_cas();
      pis_jednomu(0, 0x07, hodiny/10);
      pis_jednomu(0, 0x02, hodiny%10);
      pis_jednomu(1,)
      _delay_ms(1000);
      pocitadlo++;
      if(pocitadlo == 10) {
        pocitadlo = 0;
        cas = true;
      }
    }
  }
  return 32;
}
