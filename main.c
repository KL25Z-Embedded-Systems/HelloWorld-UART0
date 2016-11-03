//***************************************************************************
// * 
// * Created on:  02 10 2016
// * Author:      Xuweu Dai  (xuewu.dai at northumbria.ac.uk)
// *
// * File:        Example Programme UART0 transmit
// *
// * Sending "Hello World!" to UART0 on Freescale FRDM-KL25Z board.
// * UART0 is connected to openSDA debug agent and has
// * a virtual connection to the host PC COM port.
// * Use a terminal software package (e.g. superterminal, Access Port, 
// *  MobaXterm, etc) to see the message "Hello World!" on a PC's screen.

// * By default in SystemInit(), FLL clock output is 20.97152 MHz.
// * Setting BDH=0, BDL=0x17, and OSR=0x0F yields 57600 Baud.

	 
// * Copyright:   (C) 2016 Northumbria University, Newcastle upon Tyne, UK.
// *
// * This program is free software: you can redistribute it and/or modify
// * it under the terms of the GNU Lesser General Public License as published by
// * the Free Software Foundation, either version 3 of the License, or
// * (at your option) any later version.
// 
// * This program is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// * GNU Lesser General Public License for more details.
// *
// %              _
// %  \/\ /\ /   /  * _  '
// % _/\ \/\/ __/__.'(_|_|_
// **************************************************************************/

#include <MKL25Z4.H>

void UART0_init(void);
void delayMs(int n);
int32_t LED_Initialize (void);
int32_t LEDrgb_On(char colourcode);
int32_t LEDrgb_Off(char colourcode);
int32_t LED123_On(int colourcode);
int32_t LED123_Off(int colourcode);
	
int main (void) {
    char msg[] = "Hello World!\r\n";
    int i;
  LED_Initialize();
	  UART0_init();
    for (i = 0; i < 14; i++) {
       while(!(UART0->S1 & 0x80)) {  }   /* wait for transmit buffer empty */
        UART0->D = msg[i]; /* send a char */
			 
			 /*
			 LEDrgb_On('g');
			 delayMs(5);
			 LEDrgb_Off('g');
			 delayMs(40);
			 */
			 
			 LED123_On((i+1)%3);
			 LED123_Off((i%3));
			 delayMs(40);
    }
		LED123_Off(1);
		LED123_Off(2);
		LED123_Off(3);
}

/* initialize UART0 to transmit at 115200 Baud */
void UART0_init(void) {
    SIM->SCGC4 |= 0x0400;   /* enable clock for UART0 */
    SIM->SOPT2 |= 0x04000000;    /* use FLL output for UART Baud rate generator */
    UART0->C2 = 0;          /* turn off UART0 while changing configurations */
    UART0->BDH = 0x00;
    UART0->BDL = 0x17;      /* 115200 Baud */
    UART0->C4 = 0x0F;       /* Over Sampling Ratio 16 */
    UART0->C1 = 0x00;       /* 8-bit data */
    UART0->C2 = 0x08;       /* enable transmit */

    SIM->SCGC5 |= 0x0200;   /* enable clock for PORTA */
    PORTA->PCR[2] = 0x0200; /* make PTA2 UART0_Tx pin */
}

/* Delay n milliseconds */
/* The CPU core clock is set to MCGFLLCLK at 41.94 MHz in SystemInit(). */

void delayMs(int n) {
    int i;
    int j;
    for(i = 0 ; i < n; i++)
        for (j = 0; j < 7000; j++) {}
}


int32_t LED_Initialize (void) {
  uint32_t i;

  SIM->SCGC5 |= ((1U << SIM_SCGC5_PORTB_SHIFT) |
                 (1U << SIM_SCGC5_PORTD_SHIFT)  );

  /* Port Initialization */
	PORTB->PCR[18]=(1U << 8);
	PORTB->PCR[19]=(1U << 8);
	PORTD->PCR[1]=(1U << 8);
	
	PORTB->ISFR &= (1U << 18);
	PORTB->ISFR &= (1U << 19);
	PORTD->ISFR &= (1U << 1);
	
	
	GPIOB->PDOR |=  (1U << 18);
	GPIOB->PDOR |=  (1U << 19);
	GPIOD->PDOR |=  (1U << 1);
	
	GPIOB->PDDR |=  (1U << 18);
	GPIOB->PDDR |=  (1U << 19);
	GPIOD->PDDR |=  (1U << 1);
  
//  LED_SetOut(0);
  return 0;
}

int32_t LEDrgb_On(char colourcode)
{
	if (colourcode=='r') 
		  PTB->PCOR  = (1U << 18);
	if (colourcode=='g') 
		  PTB->PCOR  = (1U << 19);
	if (colourcode=='b') 
		  PTD->PCOR  = (1U << 1);
	if (colourcode=='a')   //amber, r+g
	    PTB->PCOR  = (3U << 18);
	return 0;
}

int32_t LEDrgb_Off(char colourcode)
{
	if (colourcode=='r') 
		  PTB->PSOR  = (1U << 18);
	if (colourcode=='g') 
		  PTB->PSOR  = (1U << 19);
	if (colourcode=='b') 
		  PTD->PSOR  = (1U << 1);
	if (colourcode=='a')   //amber, r+g
	    PTB->PSOR  = (3U << 18);
	return 0;
}

int32_t LED123_On(int colourcode)
{
	if (colourcode==1) 
		  PTB->PCOR  = (1U << 18);
	if (colourcode==2) 
		  PTB->PCOR  = (1U << 19);
	if (colourcode==3) 
		  PTD->PCOR  = (1U << 1);
	if (colourcode==4)   //amber, r+g
	    PTB->PCOR  = (3U << 18);
	return 0;
}

int32_t LED123_Off(int colourcode)
{
	if (colourcode==1) 
		  PTB->PSOR  = (1U << 18);
	if (colourcode==2) 
		  PTB->PSOR  = (1U << 19);
	if (colourcode==3) 
		  PTD->PSOR  = (1U << 1);
	if (colourcode==4)   //amber, r+g
	    PTB->PSOR  = (3U << 18);
	return 0;
}
