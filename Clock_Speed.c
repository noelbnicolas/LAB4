/*
 * Clock_Speed.c
 *
 *  Created on: Oct 25, 2013
 *      Author: C15Noel.Nicolas
 */

void CS1MHz(){
	BCSTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
}
void CS5MHz(){
	BCSTL1 = CALBC1_5MHZ;
	DCOCTL = CALDCO_5MHZ;
}
void CS10MHz(){
	BCSTL1 = CALBC1_10MHZ;
	DCOCTL = CALDCO_10MHZ;
}


