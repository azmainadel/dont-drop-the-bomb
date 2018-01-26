/*
 * matrix.h
 *
 * Created: 12/31/2017 3:45:05 PM
 *  Author: Lennon
 */ 
#ifndef MATRIX_H_
#define MATRIX_H_
#define F_CPU 10000UL
#include <avr/io.h>
#include<util/delay.h>
#include "shift74hc595.h"

int pl1_score=0;
int pl2_score=0;

int bar2_X=2,bar2_Y=16;
int bar1_X=2,bar1_Y=0;

uint8_t row1=0b00000001, grn1=0b11000011, red1=0b11111111;
uint8_t row2=0b10000000, grn2=0b11000011, red2=0b11111111;


int ball_X=4,ball_Y=9,dir=1;
uint8_t ball_row1=0b00000000, ball_grn1=0b11111111, ball_red1=0b11101111;
uint8_t ball_row2=0b00000001, ball_grn2=0b11111111, ball_red2=0b11101111;

void init_score(){
	pl1_score=0;pl2_score=0;
}

void init_Ball(){
	ball_X=4; ball_Y=9; dir=1;
	ball_row1=0b00000000; ball_grn1=0b11111111; ball_red1=0b11101111;
	ball_row2=0b00000001; ball_grn2=0b11111111; ball_red2=0b11101111;
}

void init_Bar(){
	bar2_X=2; bar2_Y=16;
	bar1_X=2; bar1_Y=0;

	row1=0b00000001; grn1=0b11000011; red1=0b11111111;
	row2=0b10000000; grn2=0b11000011; red2=0b11111111;
}

void setLCD(){
	Lcd4_Clear();
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String("Game ");
	Lcd4_Set_Cursor(2,6);
	Lcd4_Write_String(" running.");
}

void check(){
	if(ball_Y>=bar2_Y){
		if(ball_X>bar2_X&&ball_X<=bar2_X+4){
	//		ball_Y--;
	//		ball_row2=(ball_row2>>1);
			if(dir==3)dir=2;
			else if(dir==4)dir=1;
		}
		else
		{
			drawAll();
			Lcd4_Clear();
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String("Player 1 wins !");
			PORTD=0xff;
			for(int i=0;i<100;i++){
				drawAll();
			}
			PORTD=0x00;
			init_Ball();
			init_Bar();
			pl1_score++;
			setLCD();
			for(int i=0;i<10;i++){
				drawAll();
			}
		}
	}
	
	if(ball_Y<=bar1_Y+1){
		if(ball_X>bar1_X &&ball_X<=bar1_X+4){
		//	ball_Y++;
		//	ball_row2=(ball_row2<<1);
			if(dir==2)dir=3;
			else if(dir==1)dir=4;
			//	_delay_ms(1000);
			}
		else{
			Lcd4_Clear();
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String("Player 2 wins !");
			PORTD=0xff;
			for(int i=0;i<100;i++){
				drawAll();
			}
			PORTD=0x00;
			init_Ball();
			init_Bar();
			pl2_score++;			
			setLCD();
			for(int i=0;i<10;i++){
				drawAll();
			}
		}
	}
}

void moveTheBall()
{
//	check();
	if(dir==1){
		if(ball_Y>1&&ball_X<8){        //down_right
			ball_Y--;
			ball_X++;
			if (ball_Y==8){
				ball_row2=0b00000000;
				ball_row1=0b10000000;
			}
			else if(ball_Y<8)ball_row1=(ball_row1>>1); //down
			else ball_row2=(ball_row2>>1);
			ball_red1=(ball_red1>>1)|0x80; //right
			ball_red2=(ball_red2>>1)|0x80; //right
			_delay_ms(50);

		}
		else if(ball_X<8){
			PORTD=0xff;
			_delay_ms(50);
			PORTD=0x00;
			dir= 4 ;//up_right
		}
		else{
			PORTD=0xff;
			_delay_ms(50);
			PORTD=0x00;
			dir=2;  //down_left
		}
	}
	
	else if(dir==2){            //down_left
		if(ball_Y>1 &&ball_X>1){
			ball_Y--;
			ball_X--;
			
			if (ball_Y==8){
				ball_row2=0b00000000;
				ball_row1=0b10000000;
			}
			else if(ball_Y<8)ball_row1=(ball_row1>>1);
			else ball_row2=(ball_row2>>1);
			
			ball_red1=(ball_red1<<1)|0x01;
			ball_red2=(ball_red2<<1)|0x01;
			_delay_ms(50);
		}
		else if (ball_Y>1){
			PORTD=0xff;
			_delay_ms(50);
			PORTD=0x00;
			dir=1; //down_right
		}
		else{
			PORTD=0xff;
			_delay_ms(50);
			PORTD=0x00;
			dir=3;  //up_left
		}
	}
	else if(dir==3){                //up_left
		if(ball_Y<16 &&ball_X>1){
			ball_Y++;
			ball_X--;
			
			if(ball_Y==9){
				ball_row1=0b00000000;
				ball_row2=0b00000001;
			}
			else if(ball_Y<9)ball_row1=(ball_row1<<1);
			else ball_row2=(ball_row2<<1);
			
			ball_red1=(ball_red1<<1)|0x01;
			ball_red2=(ball_red2<<1)|0x01;
			_delay_ms(50);
		}
		else if(ball_X>1){
			PORTD=0xff;
			_delay_ms(50);
			PORTD=0x00;
			dir=2; //down_left
		}
		else{
			PORTD=0xff;
			_delay_ms(50);
			PORTD=0x00;
			dir=4;  //up_right
		}
	}
	else{                            //up_right
		if(ball_Y<16 &&ball_X<8){
			ball_Y++;
			ball_X++;
			if(ball_Y==9){
				ball_row1=0b00000000;
				ball_row2=0b00000001;
			}
			else if(ball_Y<9)ball_row1=(ball_row1<<1); //up
			else ball_row2=(ball_row2<<1);
			ball_red1=(ball_red1>>1)|0x80; //right
			ball_red2=(ball_red2>>1)|0x80; //right
			_delay_ms(50);
		}
		else if(ball_Y<16) 
		{
			PORTD=0xff;
			_delay_ms(50);
			PORTD=0x00;
			dir=3 ;//up_left
		}
		else {
			PORTD=0xff;
			_delay_ms(50);
			PORTD=0x00;
			dir=1; //down_right
		}
	}
	check();
}

void drawAll()
{

		HC595Write(row1,red1,grn1,row2,red2,grn2);
		HC595Write(ball_row1,ball_red1,ball_grn1,ball_row2,ball_red2,ball_grn2);
	//	_delay_ms(100);
}

void goUp2()
{
	if(bar2_Y>9)
	{
		row2>>=1;
		bar2_Y--;
	}
	_delay_ms(10);
}

void goDown2()
{
	if(bar2_Y<16)
	{
		row2<<=1;
		bar2_Y++;
	}
	_delay_ms(10);
}

void goLeft2()
{
	if(bar2_X<4)
	{
		grn2=(grn2>>1)|0x80;
		
		bar2_X++;
	}
	_delay_ms(10);
}

void goRight2()
{
	if(bar2_X>0)
	{
		grn2=(grn2<<1)|0x01;
		bar2_X--;
	}
	_delay_ms(10);
}

void goUp1()
{
	if(bar1_Y<7)
	{
		row1<<=1;
		bar1_Y++;
	}
	_delay_ms(10);
}

void goDown1()
{
	if(bar1_Y>0)
	{
		row1>>=1;
		bar1_Y--;
	}
	_delay_ms(10);
}

void goLeft1()
{
	if(bar1_X>0)
	{
		grn1=(grn1<<1)|0x01;
		bar1_X--;
	}
	_delay_ms(10);
}

void goRight1()
{
	if(bar1_X<4)
	{
		grn1=(grn1>>1)|0x80;
		
		bar1_X++;
	}
	_delay_ms(10);
}

#endif