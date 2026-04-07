#include <avr/io.h>
#include <util/delay.h>

#define DIN 0x01 //PORTB0
#define CS 0x02 //PORTB1
#define CLK 0x04 // PORTB2
#define SPI_DELAY 20
void setup() {
  DDRB = 0b11111111;
  PORTB = 0b00000111;
}

void spi_start() {
  PORTB &= ~(CS);
  _delay_us(SPI_DELAY);
}
void spi_stop() {
  PORTB |= CS;
  _delay_us(SPI_DELAY);
}

void spi_pis(uint8_t data) {
  if(data == 1) {
    PORTB |= DIN;
    _delay_us(SPI_DELAY);
  }
  else {
    PORTB &= ~(DIN);
    _delay_us(SPI_DELAY);
  }
    PORTB |= CLK;
    _delay_us(SPI_DELAY);
    PORTB &= ~(DIN);
    _delay_us(SPI_DELAY);
    PORTB &= ~(CLK);
    _delay_us(SPI_DELAY);
}

void vypln() {

}

void intenzita() {
  int data [16];
  adresa[0] = 1;
  adresa[1] = 0;
  adresa[2] = 1;
  adresa[3] = 0;
  spi_start();
  for(int i = 0; i <2; i++) {
    for(int i = 0; i < 4; i++) {
      spi_pis(adresa[i]);
    }
  }
  for(int i = 0; i <2; i++) {
    for(int i = 0; i < 4; i++) {
      spi_pis(1);
    }
  }
  spi_stop();
}

void zobrazeni (int data, bool konec) { //table 6 v datasheetu
  int zobrazeni[8];
  if(data == 1) {
    zobrazeni[1] =;
    zobrazeni[2] =;
    zobrazeni[3] =;
    zobrazeni[4] =;
    zobrazeni[5] =;
    zobrazeni[6] =;
    zobrazeni[7] =;
  }
  else if(data == 2) {
    zobrazeni[1] =;
    zobrazeni[2] =;
    zobrazeni[3] =;
    zobrazeni[4] =;
    zobrazeni[5] =;
    zobrazeni[6] =;
    zobrazeni[7] =;
  }
  else if(data == 2) {
    zobrazeni[1] =;
    zobrazeni[2] =;
    zobrazeni[3] =;
    zobrazeni[4] =;
    zobrazeni[5] =;
    zobrazeni[6] =;
    zobrazeni[7] =;
  }
  else if(data == 2) {
    zobrazeni[1] =;
    zobrazeni[2] =;
    zobrazeni[3] =;
    zobrazeni[4] =;
    zobrazeni[5] =;
    zobrazeni[6] =;
    zobrazeni[7] =;
  }
  else if(data == 2) {
    zobrazeni[1] =;
    zobrazeni[2] =;
    zobrazeni[3] =;
    zobrazeni[4] =;
    zobrazeni[5] =;
    zobrazeni[6] =;
    zobrazeni[7] =;
  }
  else if(data == 2) {
    zobrazeni[1] =;
    zobrazeni[2] =;
    zobrazeni[3] =;
    zobrazeni[4] =;
    zobrazeni[5] =;
    zobrazeni[6] =;
    zobrazeni[7] =;
  }
  else if(data == 2) {
    zobrazeni[1] =;
    zobrazeni[2] =;
    zobrazeni[3] =;
    zobrazeni[4] =;
    zobrazeni[5] =;
    zobrazeni[6] =;
    zobrazeni[7] =;
  }
  else if(data == 2) {
    zobrazeni[1] =;
    zobrazeni[2] =;
    zobrazeni[3] =;
    zobrazeni[4] =;
    zobrazeni[5] =;
    zobrazeni[6] =;
    zobrazeni[7] =;
  }
  if(konec == true) {
    zobrazeni[8] = 1;
  }
  else if(konec == false) {
    zobrazeni[]
  }
}

int main() {
  while(1) {
    setup();
    intenzita();
    spi_start();
    //pole adresy možná
    for(int i = 0; i < 4; i++) { //prázdné 4 horní bity
      spi_pis(0);
    }
    for(int i = 0; i < 4; i++) { //adresa

    }
    for(int i = 0; i < 8; i++) { //data

    }
  }
  return 32;
}
