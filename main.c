// **************************************************************************/
// *
// * Created on:  01 10 2016
// * Author:      Xuweu Dai  (xuewu.dai at northumbria.ac.uk)
// *
// * File:        Example Programme UART0 receiver
// *
// * Receiving any key from the PC (via a terminal emulator, e.g. 
// * hyperterminal) to the UART0 on Freescale FRDM-KL25Z board.
// * UART0 is connected to openSDA debug agent and has
// * a virtual connection to the host PC COM port.
// * Launch Hypterterminal on a PC and hit any key.
// * Different clour LEDs will be turned on or off according 
// * to the value of the letter reeived by the KL25Z
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
void LED_init(void);
void LED_set(int value);

void delayMs(int n);

int main (void) {
    char c;
int i;
    UART0_init();
    LED_init();
    
	for (i=1;i<8; i++)
	{
		LED_set(i%4);
		delayMs(500);
	}
		
    while (1) {
        while(!(UART0->S1 & 0x20)) {
        }   /* wait for receive buffer full */
        c = UART0->D; /* read the char received */
        LED_set(c);
    }
}

/* initialize UART0 to receive at 115200 Baud */
void UART0_init(void) {
    SIM->SCGC4 |= 0x0400;   /* enable clock for UART0 */
    SIM->SOPT2 |= 0x04000000;    /* use FLL output for UART Baud rate generator */
    UART0->C2 = 0;          /* turn off UART0 while changing configurations */
    UART0->BDH = 0x00;
    UART0->BDL = 0x17;      /* 115200 Baud */
    UART0->C4 = 0x0F;       /* Over Sampling Ratio 16 */
    UART0->C1 = 0x00;       /* 8-bit data */
    UART0->C2 = 0x04;       /* enable receive */

    SIM->SCGC5 |= 0x0200;   /* enable clock for PORTA */
    PORTA->PCR[1] = 0x0200; /* make PTA1 UART0_Rx pin */
}

/* initialize all three LEDs on the FRDM board */
void LED_init(void)
{
    SIM->SCGC5 |= 0x400;        /* enable clock to Port B */
    SIM->SCGC5 |= 0x1000;       /* enable clock to Port D */
    PORTB->PCR[18] = 0x100;     /* make PTB18 pin as GPIO */
    PTB->PDDR |= 0x40000;       /* make PTB18 as output pin */
    PTB->PSOR = 0x40000;        /* turn off red LED */
    PORTB->PCR[19] = 0x100;     /* make PTB19 pin as GPIO */
    PTB->PDDR |= 0x80000;       /* make PTB19 as output pin */
    PTB->PSOR = 0x80000;        /* turn off green LED */
    PORTD->PCR[1] = 0x100;      /* make PTD1 pin as GPIO */
    PTD->PDDR |= 0x02;          /* make PTD1 as output pin */
    PTD->PSOR = 0x02;           /* turn off blue LED */
}

/* turn on or off the LEDs according to bit 2-0 of the value */
void LED_set(int value)
{
    if (value & 1)    /* use bit 0 of value to control red LED */
        PTB->PCOR = 0x40000;    /* turn on red LED */
    else
        PTB->PSOR = 0x40000;    /* turn off red LED */
    
    if (value & 2)    /* use bit 1 of value to control green LED */
        PTB->PCOR = 0x80000;    /* turn on green LED */
    else
        PTB->PSOR = 0x80000;    /* turn off green LED */
    
    if (value & 4)    /* use bit 2 of value to control blue LED */
        PTD->PCOR = 0x02;       /* turn on blue LED */
    else
        PTD->PSOR = 0x02;       /* turn off blue LED */
}

/* Delay n milliseconds
 * The CPU core clock is set to MCGFLLCLK at 41.94 MHz in SystemInit().
 */
void delayMs(int n) {
    int i;
    int j;
    for(i = 0 ; i < n; i++)
        for(j = 0 ; j < 7000; j++) { }
}