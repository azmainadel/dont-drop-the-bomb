/*
 * GyroOne.c
 *
 * Created: 13-May-17 9:20:32 PM
 *  Author: HP
 */ 

#include<stdio.h>
#include<stdlib.h>

#include<avr/io.h>
#include<avr/interrupt.h>
#include<avr/pgmspace.h>



#ifndef F_CPU
#define F_CPU 1006000
#endif
#define D4 eS_PORTA3
#define D5 eS_PORTA2
#define D6 eS_PORTA1
#define D7 eS_PORTA0
#define RS eS_PORTA5
#define EN eS_PORTA4

#include <util/delay.h>
#include <math.h>  //include libm

#include "mpu6050/mpu6050.h"
#include "matrix.h"
#include "lcd.h"



#define MPU6050_GETATTITUDE 0


void vertical1(int16_t ax){
	
	if(ax < 0){
		goUp1();
	}
	else if(ax > 0){
		goDown1();
	}
}
 
void horizontal1(int16_t ay){	
	if(ay < 0){
		goRight1();
	}
	else if(ay > 0){
		goLeft1();
	}
}

void vertical2(int16_t ax){
	
	if(ax < 0){
		goUp2();
	}

	else if(ax > 0){
		goDown2();
	}
}

void horizontal2(int16_t ay){
	if(ay < 0){
		goRight2();
	}
	else if(ay > 0){
		goLeft2();
	}
}




int main(void) {
	
	DDRA = 0xFF;
	DDRD =0xFF;

	
	ADMUX = 0b00100000;
	ADCSRA = 0b10000010;


	Lcd4_Init();
	drawAll();
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Welcome o_O");
	_delay_ms(800);
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Don't drop");
	_delay_ms(2000);
	Lcd4_Set_Cursor(2,4);
	Lcd4_Write_String("the bomb -_-");
	_delay_ms(2000);
	
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("<-Player 2");
	_delay_ms(800);
	Lcd4_Set_Cursor(2,6);
	Lcd4_Write_String("Player 1->");
	_delay_ms(2000);

	Lcd4_Clear();
	Lcd4_Set_Cursor(1,3);
	Lcd4_Write_String("Ready ?");
	_delay_ms(2000);
	Lcd4_Set_Cursor(2,6);
	Lcd4_Write_String("Go !");
	_delay_ms(1000);

	setLCD();
	
	
	
	GyroAddr = (0x69 << 1);
//	GyroAddr = (0x68 << 1);


	#if MPU6050_GETATTITUDE == 0
	int16_t ax = 0;
	int16_t ay = 0;
	int16_t az = 0;
	int16_t gx = 0;
	int16_t gy = 0;
	int16_t gz = 0;
	double axg = 0;
	double ayg = 0;
	double azg = 0;
	double gxds = 0;
	double gyds = 0;
	double gzds = 0;
	#endif

	#if MPU6050_GETATTITUDE == 1 || MPU6050_GETATTITUDE == 2
	long *ptr = 0;
	double qw = 1.0f;
	double qx = 0.0f;
	double qy = 0.0f;
	double qz = 0.0f;
	double roll = 0.0f;
	double pitch = 0.0f;
	double yaw = 0.0f;
	#endif

	//init uart
	//uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));

	//init interrupt
	sei();

	//init mpu6050
	mpu6050_init();
	_delay_ms(50);

	//init mpu6050 dmp processor
	#if MPU6050_GETATTITUDE == 2
	mpu6050_dmpInitialize();
	mpu6050_dmpEnable();
	_delay_ms(10);
	#endif
	
	int which = 0;
	
	HC595Init();
	
	extern	int pl1_score,pl2_score;
	while(1) {
		
		if(which)
		{
			GyroAddr = (0x69 << 1);
		}
		else
		{
			GyroAddr = (0x68 << 1);
		}
		
		//#if MPU6050_GETATTITUDE == 0
		mpu6050_getRawData(&ax, &ay, &az, &gx, &gy, &gz);
		//mpu6050_getConvData(&axg, &ayg, &azg, &gxds, &gyds, &gzds);
		//#endif
		
		ax = -ax;
		
		//stay();
		if(which){
			vertical1(ax);
			horizontal1(ay);
		}else
		{
			vertical2(ax);
			horizontal2(ay);
		}
		drawAll();
		moveTheBall();
		
		
		if(pl1_score==5||pl2_score==5){
			init_score();
			init_Ball();
			init_Bar();
			Lcd4_Clear();
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String("Game OVER");
			Lcd4_Set_Cursor(2,0);
			if(pl1_score==5)Lcd4_Write_String("Player 1 wins!!");
			else Lcd4_Write_String("Player 2 wins!!");
			for(int i=0;i<1000;i++){
				drawAll();
			}
			setLCD();
		}		
		
		
		Lcd4_Set_Cursor(1,6);
		Lcd4_Write_Char('0'+pl1_score);
		Lcd4_Write_Char('-');
		Lcd4_Write_Char('0'+pl2_score);
		Lcd4_Set_Cursor(2,5);
		Lcd4_Write_Char('0'+pl1_score+pl2_score+1);
		
		
		
		
	//	if(which) twoA(ax);
	//	else twoB(ax);
		
		//twoB(ax);
		
		which = 1 - which;
		
		
		//PORTA = ax & 0xFF;
		
		//_delay_ms(10);
	}




}
