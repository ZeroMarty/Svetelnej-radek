#include <avr/io.h>
#include <util/delay.h>
//MAX7219
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

void prevodbinar() {

}

void spi_pis(uint16_t data) {
  int i = 0;
  for( i = 0; i < 16; i++) {
    if(data & 0x8000 == 0) {
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
    data << 1;
  }
}

void scan() {
  uint16_t data = 0xFB07;
  spi_start();
  spi_pis(data);
  spi_stop();
}

void test() {
  uint16_t data = 0xF807;
  spi_start();
  spi_pis(data);
  spi_stop();
}

void spi_vypni() {
  uint16_t data = 0xFCF0;
  spi_start();
  spi_pis(data);
  spi_stop();
}
void spi_zapni() {
  uint16_t data = 0xFCF1;
  spi_start();
  spi_pis(data);
  spi_stop();
}

void intenzita() {
  uint16_t data = 0xFAFF;
  spi_start();
  spi_pis(data);
  spi_stop();
}

void decode() {
  uint16_t data = 0xF900;
  spi_start();
  spi_pis(data);
  spi_stop();
}
void setup_spi() {
  spi_vypni();
  spi_zapni();
  scan();
  decode();
  intenzita();
}

void zobrazeni (int data, bool konec) { //table 6 v datasheetu
  
}

int main() {
  setup();
  setup_spi();
  test();
  while(1) {
   
  }
  return 32;
}
