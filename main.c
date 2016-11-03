//***************************************************************************
// * 
// * Created on:  01 10 2016
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
	
int main (void) {
    char msg[] = "Hello World!\r\n";
    int i;
  
    UART0_init();
    for (i = 0; i < 14; i++) {
       while(!(UART0->S1 & 0x80)) {  }   /* wait for transmit buffer empty */
        UART0->D = msg[i]; /* send a char */
  	    delayMs(40);
    }
}

/* initialize UART0 to transmit at 57600 Baud */
void UART0_init(void) {
    SIM->SCGC4 |= 0x0400;   /* enable clock for UART0 */
    SIM->SOPT2 |= 0x04000000;    /* use FLL output for UART Baud rate generator */
    UART0->C2 = 0;          /* turn off UART0 while changing configurations */
    UART0->BDH = 0x00;
    UART0->BDL = 0x17;      /* 57600 Baud */
    UART0->C4 = 0x0F;       /* Over Sampling Ratio 16 */
    UART0->C1 = 0x00;       /* 8-bit data */
    UART0->C2 = 0x08;       /* enable transmit */

    SIM->SCGC5 |= 0x0200;   /* enable clock for PORTA */
    PORTA->PCR[2] = 0x0200; /* make PTA2 UART0_Tx pin */
}

/* Delay n milliseconds */
/* The CPU core clock is set to MCGFLLCLK at 20.97152 MHz in SystemInit(). */
void delayMs(int n) {
    int i;
    int j;
    for(i = 0 ; i < n; i++)
        for (j = 0; j < 3500; j++) {}
}