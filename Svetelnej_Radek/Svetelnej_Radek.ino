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
#define pocet_sloupcu 96

uint8_t hodiny = 14;
uint8_t minuty = 25;
uint8_t sekundy = 30;
uint8_t den = 21;
uint8_t mesic = 4;
uint16_t rok = 2026;
uint8_t denvtydnu = 2;

uint8_t display[pocet_sloupcu]; //framebuffer displaye

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

uint8_t pole (char c) { //překlad znaků na index v poli v fontu
  if(c>='0' && c<='9') return c-'0';
  if(c == ':') return 10;
  if(c=='.') return 11;
  if(c=='-') return 12;
  return 13;
}

void spi_pis(uint8_t addr, uint8_t data) {
  for(int i = 0; i < 8; i++) {
    if((addr & 0x80) == 0) {
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
    if((data & 0x80) == 0) {
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

void prehoz(uint8_t *a, uint8_t *b) {
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
    denvtydnu++;
    prehoz(&hodiny, &den);
  }
  if(den == 31) {
    prehoz(&den, &mesic);
  }
  if(mesic == 13) {
    mesic = 1;
    rok++;
  }
  if(denvtydnu == 8) {
    denvtydnu = 1;
  }
}

void smaz() {
  for(int i = 0; i < pocet_sloupcu; i++) {
    display[i] = 0;
  }
}

void obnova() {
  for (uint8_t sloupec = 1; sloupec <= 8; sloupec++) { //kvůli noop začínám v 1
    spi_start();
    for(int i = 11; i >= 0; i--) { //odzadu
      spi_pis(sloupec, display[(i*8) + (sloupec-1)]);
    }
    spi_stop();
  }
}

void vykresli(char c, int start) {
  uint8_t index = pole(c);
  for(int sloupec = 0; sloupec <8;sloupec++){
    uint8_t data = 0;
    for(int radek = 0; radek < 8; radek++) {
      if (font8x8_basic[(uint8_t)c][radek] & (1 << (7-sloupec))) {
        data |= (1 << (7 - radek));
      }
    }
    if(start + sloupec >=0 && start + sloupec < pocet_sloupcu) {
      display[start +sloupec] = data;
    }
  }
}

void vypis(const char *znaky, int start, int mezera) {
  int pozice = start;
  int i = 0;
  while(znaky[i]!='\0') {
    vykresli(znaky[i], pozice);
    pozice += 8 + mezera; //Velikost znaku 8 a mezera
  }
}

int main() {
  char text [50];
  bool cas = true; //datum/čas přehoz
  int pocitadlo = 0;
  setup();
  setup_spi();  
  while(1) {
    while(cas == true) {
      update_cas();
      smaz();
      sprintf(text, "02d%:02d%:02d%", hodiny, minuty, sekundy);
      vypis(text, 0, 2);
      obnova();
      _delay_ms(1000);
      pocitadlo++;
      if(pocitadlo == 10) {
        pocitadlo = 0;
        cas = false;
      }
    }
    while(cas == false) {
      update_cas();
      smaz();
      sprintf(text ,"%02d/%02d/%04d/%02d", den, mesic, rok, denvtydnu);
      vypis(text, 0, 2);
      obnova();
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
