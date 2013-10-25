#include <msp430.h>
#include "LCD.h"

/*
 * main.c
 */ // Note to self |= BIT SET, &= BIT CLEAR, var <<4; shifts var 4 places to the left



//int ButtonHit = 0;


int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	char * Msg1 = "ECE 382 is my favorite class    ";
	char * Msg2 = "I haven't slept in days    ";
	char * Msg3 = "I've been eating nutella    ";
	char * Msg4 = "I hope I don't get diabetes    ";
	char * BeginMsgLine1 = "Message?";
	char * BeginMsgLine2 ="Press123";

	initSPI();
	LCDinit();
	LCDclear();

	cursorToLineOne();
	writeString(BeginMsgLine1);
	cursorToLineTwo();
	writeString(BeginMsgLine2);

	int HitButton = 0;
	HitButton = checkButton();


	if(HitButton ==1){
		scrollString(Msg1, Msg2);
}
	if(HitButton ==2){
			scrollString(Msg1, Msg3);

}
	if(HitButton ==3){
			scrollString(Msg1, Msg4);

}
	return(0);
}
