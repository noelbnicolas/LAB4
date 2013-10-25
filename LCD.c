/*
 * main.c
 *
 *  Created on: Oct 24, 2013
 *      Author: C15Noel.Nicolas
 */
#include <msp430.h>
#include "LCD.h"
#define RS_MASK 0x40
char LCDCON = 0;

void set_SS_LO(){
	P1OUT &=~ BIT0;
	P1DIR |= BIT0;
}

void set_SS_hi(){
	P1OUT |= BIT0;
	P1DIR |= BIT0;
}

void initSPI(){
	UCB0CTL1 |= UCSWRST|UCSSEL1;
	UCB0CTL0 |= UCCKPH|UCMSB|UCMST|UCSYNC;
	UCB0STAT |= UCLISTEN;
	P1DIR |= BIT0;
	P1SEL |= BIT5|BIT6|BIT7;
	P1SEL2 |= BIT5|BIT6|BIT7;
	UCB0CTL1 &= ~UCSWRST;

}

void writeCommandNibble(char commandNibble);
void writeCommandByte(char commandByte);
void SPI_send(char byteToSend);
void delayMicro();

void LCDinit(){
    writeCommandNibble(0x03);

    writeCommandNibble(0x03);

    writeCommandNibble(0x03);

    writeCommandNibble(0x02);

    writeCommandByte(0x28);

    writeCommandByte(0x0C);

    writeCommandByte(0x01);

    writeCommandByte(0x06);

    writeCommandByte(0x01);

    writeCommandByte(0x02);

    SPI_send(0);
    delayMicro();
}


void cursorToLineTwo(){
	writeCommandByte(0xC0);
//	LCDCON |= RS_MASK;
//	delayMicro();

}
void cursorToLineOne(){
	writeCommandByte(0x80);
//	LCDCON |= RS_MASK;
//	delayMicro();
}

void LCDclear()
{
    writeCommandByte(1);
    LCDCON |= RS_MASK;
    delayMicro();
}

void LCD_write_8(char byteToSend);
void delayMilli();

void writeDataByte(char dataByte)
{
    LCDCON |= RS_MASK;
    LCD_write_8(dataByte);
    delayMilli();
}


void writeString(char * string){
	unsigned int i;
	LCDCON |= RS_MASK;
	for(i=0; i<8; i++){
		LCD_write_8(string[i]);
		delayMicro();

	}
}

void scrollString(char * string1,char * string2){
	unsigned int i;
	char *LineOneCount = string1, *LineTwoCount = string2;
	while(1){
		cursorToLineOne();
		char *Charnow= LineOneCount;
		for(i=0; i<8; i++){
			writeDataByte(*Charnow);
			Charnow++;
			if(*Charnow == 0){
				Charnow = string1;
			}
			LineOneCount++;
			if(*LineOneCount == 0){
				LineOneCount = string1;
			}
		}


		cursorToLineTwo();

		char *Charnow2= LineTwoCount;

		for(i=0; i<8; i++){

			writeDataByte(*Charnow2);

			Charnow2++;

			if(*Charnow2== 0){

				Charnow2 = string2;

			}

			LineTwoCount++;

			if(*LineTwoCount == 0){

				LineTwoCount = string2;

			}
		}
		_delay_cycles(600000);
		LCDclear();


	}
}
void LCD_write_4(char sendByte);


void scrollMessage(char *string1, char *string2) {
        unsigned int i = 0;

        char *count1 = string1, *count2 = string2;

        while (1) {
                cursorToLineOne();
                char *currentChar = count1;

                for (i = 0; i < 8; i++) {
                     writeDataByte(*currentChar);        //send data in the string to be written

                     currentChar++;

                     if (*currentChar == 0)
                           currentChar = string1;
                }
                count1++;

                if (*count1 == 0) {
                     count1 = string1;
                }

                cursorToLineTwo();
                char *currentChar2 = count2;
                for (i = 0; i < 8; i++) {
                      writeDataByte(*currentChar2);        //send data in the string to be written

                      currentChar2++;

                      if (*currentChar2 == 0)
                      currentChar2 = string2;
                }
                count2++;

                if (*count2 == 0) {
                    count2 = string2;
                }

                __delay_cycles(600000);

                LCDclear();
        }
}


//void scrollMessage(char *string, char *LineOneCount){
//	unsigned int i = 0;
//	char *ScrollChar = LineOneCount;
//	for(i=0; i <8; i++){
//		writeDataByte(*ScrollChar);
//		ScrollChar++;
//		if(*ScrollChar==0)
//				ScrollChar = string;
//		}
//	LineOneCount++;
//	if(*LineOneCount == 0){
//		LineOneCount= string;
//	}
//}

void delayMicro(){
	__delay_cycles(60);
}


void delayMilli(){
	__delay_cycles(2000);
}



void writeCommandNibble(char commandNibble){
    LCDCON &= ~RS_MASK;
    LCD_write_4(commandNibble);
    delayMilli();
}

void writeCommandByte(char commandByte){
    LCDCON &= ~RS_MASK;
    LCD_write_8(commandByte);
    delayMilli();
}

void LCD_write_4(char sendByte){
	sendByte &= 0x0F;
	sendByte |= LCDCON;
	sendByte &= 0x7F;
	SPI_send(sendByte);
	delayMicro();
	sendByte |= 0x80;
	SPI_send(sendByte);
	delayMicro();
	sendByte &= 0x7F;
	SPI_send(sendByte);
	delayMicro();
}

void LCD_write_8(char byteToSend){//Rotate right >>
	unsigned char sendByte = byteToSend;
	sendByte &= 0xF0;
	sendByte = sendByte >> 4;
	LCD_write_4(sendByte);
	sendByte= byteToSend;
	sendByte &= 0x0F;
	LCD_write_4(sendByte);
}
void SPI_send(char byteToSend){
    volatile char readByte;

    set_SS_LO();

    UCB0TXBUF = byteToSend;

    while(!(UCB0RXIFG & IFG2)){

    }

    readByte = UCB0RXBUF;

    set_SS_hi();
}



void writeChar(char asciiChar){
	writeDataByte(asciiChar);
}

int checkButton(){
	int Buttoncheck = 0;
	while(Buttoncheck==0){
	if((P1IN & BIT1) == 0){
		Buttoncheck = 1;
	return(Buttoncheck);
		}
	if((P1IN & BIT2) == 0){
		Buttoncheck = 2;
		return(Buttoncheck);
		}
	if((P1IN & BIT3) == 0){
		Buttoncheck = 3;
		return(Buttoncheck);
		}
}
	return(Buttoncheck);
}

