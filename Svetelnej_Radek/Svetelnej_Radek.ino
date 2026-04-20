#include <avr/io.h>
#include <util/delay.h>
//MAX7219
//SPI končí až kdyź končí, Ovládej v tomto pořadí: Shutdown -> Decode -> Scan limit -> Intenzita -> Display test
#define DIN 0x01 //PORTB0
#define CS 0x02 //PORTB1
#define CLK 0x04 // PORTB2
#define SPI_DELAY 50
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
// start stop funguje
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
    data = data << 1;
  }
}

void scan() {
  spi_pis(0xFB00);
}

void test() {
  spi_pis(0xF101);
  spi_pis(0xF200);
  spi_pis(0xF300);
  spi_pis(0xF400);
  spi_pis(0xF500);
  spi_pis(0xF600);
  spi_pis(0xF700);
  spi_pis(0xF800);
}

void spi_vypni() {
  spi_pis(0xFCF0);
}

void spi_zapni() {
  spi_pis(0xFCF1);
}

void display_test() {
  spi_pis(0xF00);
}

void intenzita() {
  spi_pis(0xFAFF);
}

void decode() {
  spi_pis(0xF9FF);
}
void setup_spi() {
  spi_vypni();
  spi_zapni();
  //scan();
  //decode();
  intenzita();
}

void zobrazeni (int data, bool konec) { //table 6 v datasheetu
  
}

int main() {
  setup();
  spi_start();
  setup_spi();  
  while(1) {
   test();
   
   spi_stop();
  }
  return 32;
}
