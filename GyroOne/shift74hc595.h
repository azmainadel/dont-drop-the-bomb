/*
 * _74595.h
 *
 * Created: 1/2/2018 11:17:08 PM
 *  Author: Lennon
 */ 

#ifndef shift74hc595_H_
#define shift74hc595_H_

#include <avr/io.h>
#include <util/delay.h>

/***************************************

Configure Connections

****************************************/

#define HC595_PORT   PORTB
#define HC595_DDR    DDRB

#define HC595_DS_POS PB0      //Data pin (DS) pin location

#define HC595_SH_CP_POS PB1      //Shift Clock (SH_CP) pin location 
#define HC595_ST_CP_POS PB2      //Store Clock (ST_CP) pin location

/***************************************
Configure Connections ***ENDS***
****************************************/

//Initialize HC595 System

void HC595Init()
{
   //Make the Data(DS), Shift clock (SH_CP), Store Clock (ST_CP) lines output
   HC595_DDR|=((1<<HC595_SH_CP_POS)|(1<<HC595_ST_CP_POS)|(1<<HC595_DS_POS));
}


//Low level macros to change data (DS)lines
#define HC595DataHigh() (HC595_PORT|=(1<<HC595_DS_POS))

#define HC595DataLow() (HC595_PORT&=(~(1<<HC595_DS_POS)))

//Sends a clock pulse on SH_CP line
void HC595Pulse()
{
   //Pulse the Shift Clock

   HC595_PORT|=(1<<HC595_SH_CP_POS);//HIGH

   HC595_PORT&=(~(1<<HC595_SH_CP_POS));//LOW

}

//Sends a clock pulse on ST_CP line
void HC595Latch()
{
   //Pulse the Store Clock

   HC595_PORT|=(1<<HC595_ST_CP_POS);//HIGH
//   _delay_ms(10);

   HC595_PORT&=(~(1<<HC595_ST_CP_POS));//LOW
  // _delay_ms(10);
}


/*

Main High level function to write a single byte to
Output shift register 74HC595. 

Arguments:
   single byte to write to the 74HC595 IC

Returns:
   NONE

Description:
   The byte is serially transfered to 74HC595
   and then latched. The byte is then available on
   output line Q0 to Q7 of the HC595 IC.

*/
void HC595Write(uint8_t row1,uint8_t red1,uint8_t grn1,uint8_t row2,uint8_t red2,uint8_t grn2)
{
   //Send each 8 bits serially

   //Order is MSB first
   for(uint16_t i=0;i<48;i++)
   {
      //Output the data on DS line according to the
      //Value of MSB
	  if(i<8){
			if(grn2 & 0x80){
		     //MSB is 1 so output high
			HC595DataHigh();
			}else{
		     //MSB is 0 so output high
			 HC595DataLow();
	      }	

		HC595Pulse();  
		grn2=grn2<<1|0x01; 
	  }
	  else if(i<16){
		  if(red2 & 0x80){
			  //MSB is 1 so output high
			  HC595DataHigh();
			  }else{
			  //MSB is 0 so output high
			  HC595DataLow();
		  }

		  HC595Pulse();
		  red2=red2<<1|0x01;
	  }
	  else if(i<24){
		  if(row2 & 0x80){
			  //MSB is 1 so output high
			  HC595DataHigh();
			  }else{
			  //MSB is 0 so output high
			  HC595DataLow();
		  }

		  HC595Pulse();
		  row2=row2<<1;
	  }
	  else if(i<32){
		  if(grn1 & 0x80){
			  //MSB is 1 so output high
			  HC595DataHigh();
			  }else{
			  //MSB is 0 so output high
			  HC595DataLow();
		  }

		  HC595Pulse();
		  grn1=grn1<<1;
	  }
	  else if(i<40){
		  if(red1 & 0x80){
			  //MSB is 1 so output high
			  HC595DataHigh();
			  }else{
			  //MSB is 0 so output high
			  HC595DataLow();
		  }

		  HC595Pulse();
		  red1=red1<<1;
	  }
	  else{
		  if(row1 & 0x80){
			  //MSB is 1 so output high
			  HC595DataHigh();
			  }else{
			  //MSB is 0 so output high
			  HC595DataLow();
		  }

		  HC595Pulse();
		  row1=row1<<1;
	  }

   }

   //Now all 8 bits have been transferred to shift register
   //Move them to output latch at one
   HC595Latch();
}

/*

Simple Delay function approx 0.5 seconds

*/

void Wait()
{
   for(uint8_t i=0;i<30;i++)
   {
      _delay_loop_2(0);
   }
}

#endif