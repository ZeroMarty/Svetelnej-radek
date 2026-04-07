#include <avr/io.h>
#include <util/delay.h>

#define DIN 0x01 //PORTB0
#define CS 0x02 //PORTB1
#define CLK 0x04 // PORTB2
//#define DELAY
void setup() {
  DDRB = 0b11111111;
  PORTB = 0b00000111;
}

void spi_init() {
  
}

int main() {
  // put your main code here, to run repeatedly:
  while(1) {

  }
  return 32;
}
