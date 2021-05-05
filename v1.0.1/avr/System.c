/*****************************************************************************
 *                                                                           *
 *  uCUnit - A unit testing framework for microcontrollers                   *
 *                                                                           *
 *  (C) 2007 - 2008 Sven Stefan Krauss                                       *
 *                  https://www.ucunit.org                                   *
 *                                                                           *
 *  File        : System.c                                                   *
 *  Description : System dependent functions used by uCUnit.                 *
 *                This file has to be customized for your hardware.         *
 *  Author      : Sven Stefan Krauss                                         *
 *  Contact     : www.ucunit.org                                             *
 *                                                                           *
 *****************************************************************************/

/*
 * This file is part of ucUnit.
 *
 * You can redistribute and/or modify it under the terms of the
 * Common Public License as published by IBM Corporation; either
 * version 1.0 of the License, or (at your option) any later version.
 *
 * uCUnit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Common Public License for more details.
 *
 * You should have received a copy of the Common Public License
 * along with uCUnit.
 *
 * It may also be available at the following URL:
 *       http://www.opensource.org/licenses/cpl1.0.txt
 *
 * If you cannot obtain a copy of the License, please contact the
 * author.
 */
#define F_CPU 8000000UL

#include <avr/io.h>

#include <stdio.h>
#include <stdlib.h>
#include "System.h"

/* ATmega with two USART */
#define ATMEGA_USART1
#define UART0_RECEIVE_INTERRUPT USART0_RX_vect
#define UART1_RECEIVE_INTERRUPT USART1_RX_vect
#define UART0_TRANSMIT_INTERRUPT USART0_UDRE_vect
#define UART1_TRANSMIT_INTERRUPT USART1_UDRE_vect
#define UART0_STATUS UCSR0A
#define UART0_CONTROL UCSR0B
#define UART0_CONTROLC UCSR0C
#define UART0_DATA UDR0
#define UART0_UDRIE UDRIE0
#define UART0_UBRRL UBRR0L
#define UART0_UBRRH UBRR0H
#define UART0_BIT_U2X U2X0
#define UART0_BIT_RXCIE RXCIE0
#define UART0_BIT_RXEN RXEN0
#define UART0_BIT_TXEN TXEN0
#define UART0_BIT_UCSZ0 UCSZ00
#define UART0_BIT_UCSZ1 UCSZ01
#define UART1_STATUS UCSR1A
#define UART1_CONTROL UCSR1B
#define UART1_CONTROLC UCSR1C
#define UART1_DATA UDR1
#define UART1_UDRIE UDRIE1
#define UART1_UBRRL UBRR1L
#define UART1_UBRRH UBRR1H
#define UART1_BIT_U2X U2X1
#define UART1_BIT_RXCIE RXCIE1
#define UART1_BIT_RXEN RXEN1
#define UART1_BIT_TXEN TXEN1
#define UART1_BIT_UCSZ0 UCSZ10
#define UART1_BIT_UCSZ1 UCSZ11

#define BAUD_RATE 9600UL
#define UART_BAUD_SELECT_DOUBLE_SPEED(baudRate, xtalCpu) (((((xtalCpu) + 4UL * (baudRate)) / (8UL * (baudRate)) - 1UL)) | 0x8000)

void uart_putc(char);

int
uart_putchar(char c, FILE* stream)
{
	uart_putc(c);
	return c;
}
static FILE mystdout;

/* Stub: Initialize your hardware here */
void System_Init(void)
{
	unsigned int baudrate;
	baudrate = UART_BAUD_SELECT_DOUBLE_SPEED(BAUD_RATE, F_CPU);
	
	fdev_setup_stream(&mystdout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &mystdout; // printf init

	    if (baudrate & 0x8000) {
		    UART0_STATUS = (1 << UART0_BIT_U2X); //Enable 2x speed
	    }
		/* Set baud rate */
		UBRR0H = (unsigned char)(((baudrate)>>8) & 0x80);
		UBRR0L = (unsigned char)  (baudrate & 0xFF);
		/* Enable receiver and transmitter */
		UCSR0B = (1<<RXEN0)|(1<<TXEN0);
		/* Set frame format: 8data, 2stop bit */
		UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
	
/*
	UART0_UBRRH = (unsigned char)((baudrate >> 8) & 0x80);
	UART0_UBRRL = (unsigned char)(baudrate & 0x00FF);
	UART0_CONTROLC = (1 << UART0_BIT_UCSZ1) | (1 << UART0_BIT_UCSZ0);
*/
	printf("Init of hardware finished.\n");
}

/* Stub: Shutdown your hardware here */
void System_Shutdown(void)
{

	/* asm("\tSTOP"); */
	printf("System shutdown.\n");
	exit(0);
}

/* Stub: Resets the system */
void System_Reset(void)
{
	/* Stub: Reset the hardware */
	/* asm("\tRESET"); */
	printf("System reset.\n");
	exit(0);
}

/* Stub: Put system in a safe state */
void System_Safestate(void)
{
	/* Disable all port pins */
	/* PORTA = 0x0000; */
	/* PORTB = 0x0000; */
	/* PORTC = 0x0000; */

	/* Disable interrupts */
	/* DIE(); */

	/* Put processor into idle state */
	/* asm("\tIDLE"); */
	printf("System safe state.\n");
	exit(0);
}

void uart_putc(char c)
{
//	putchar(c);
	while (!(UCSR0A & (1 << UDRE0))){}
	UDR0 = c;
}


/* Stub: Transmit a string to the host/debugger/simulator */
void System_WriteString(char * s)
{
	while(*s)
	{
		uart_putc(*s);
		s++;
	}
}

void System_WriteInt(int n)
{
	/* printf("%i", n); */
}

