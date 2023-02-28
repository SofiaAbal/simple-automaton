#include "MKL46Z4.h"

void delay(void)
{
  volatile int i;

  for (i = 0; i < 1000000; i++);
}

void led_green_init()
{
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;	
  PORTD->PCR[5] = PORT_PCR_MUX(1);
  GPIOD->PDDR |= (1 << 5); 
  GPIOD->PSOR = (1 << 5);
}

void led_green_toggle()
{
  GPIOD->PTOR = (1 << 5);
}

void led_red_init()
{
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
  PORTE->PCR[29] = PORT_PCR_MUX(1);
  GPIOE->PDDR |= (1 << 29);
  GPIOE->PSOR = (1 << 29);
}

void led_red_toggle()
{
  GPIOE->PTOR = (1 << 29);
}

void config_input_sw1(){
  SIM->COPC = 0;  // disable watchdog
  SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK; // power C port
  PORTC->PCR[3] = PORT_PCR_MUX(1);  // bit 3 of port C as input
  GPIOC->PDDR |= (1 << 3); // mask bit 3 port C
  GPIOC->PSOR = (1 << 3);
}

int is_sw1_pressed(){
  return (GPIOC->PDIR & (1 << 3)) == 0; //  checks if bit 3 is zero (& checks bit by bit)
}

void config_input_sw2(){
  SIM->COPC = 0;  // disable watchdog
  SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK; // power C port
  PORTC->PCR[12] = PORT_PCR_MUX(1);  // bit 12 of port C as input
  GPIOC->PDDR |= (1 << 12); // mask bit 12 port C
  GPIOC->PSOR = (1 << 12);
}

int is_sw2_pressed(){
  return (GPIOC->PDIR & (1 << 12)) == 0; //  checks if bit 12 is zero
}

void set_up(){
  config_input_sw1();
  config_input_sw2();
  led_green_init();
  led_red_init();
}

int main(void)
{
  int sw1_pressed = 0, sw2_pressed = 0, both_closed = 1; // initially both closed
  set_up();
  while (1) {
    // checks sw1
    if (is_sw1_pressed()){
      sw1_pressed++;
      if (sw1_pressed == 1){
        // sw1 open
        both_closed = 0;
      }else{
        // sw1 closed
        sw1_pressed = 0;
      }
    }

    // checks sw2
    if (is_sw2_pressed()){
      sw2_pressed++;
      if (sw2_pressed == 1){
        // sw2 open
        both_closed = 0;
      }else{
        // sw2 closed
        sw2_pressed = 0;
      }
    }
  }

  if (both_closed){
    led_green_toggle();
  } else{
    led_red_toggle();
  }

  delay();

  return 0;
}
