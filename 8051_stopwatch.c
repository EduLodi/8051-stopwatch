#pragma SMALL
#include <reg51.h>

#define CMD 0
#define DADO 1

//interrupcoes externas
void ext0(void);
void ext1(void);

//funcoes para aumentar o tempo e atualizar o display
void timeIncrease(void);
void updtCurr(void);
void updtDiff(void);
void updtLast(void);
void updtSmall(void);
void updtBig(void);

//func para reset
void resetSist(void);

//funcoes para controle do LCD
void delay(void);
void Init_LCD(void);
void position(unsigned char line, unsigned char col);
void send_byte(unsigned char byte, unsigned char cmd_msg);
void wrt_num1(unsigned char number1);
void wrt_num2(unsigned char number2);

sbit RS = P1^3;
sbit EN = P1^2;

unsigned char X1 = 0;
unsigned char X2, X3;
unsigned char maxX3 = 0;
unsigned char minX3 = 999;
unsigned char inc = 0;
unsigned char lajj = 0;
unsigned char rstval = 0;

void main(){
	TMOD = 0x01; // 1GATE C/T M1 M0 0GATE C/T M1 M0
	
	TH0 = 0x97;  //25000 ciclos
	TL0 = 0x05;	
	
	ET0 = 1; // HABILITA INTERRUPCAO 1 (timer)
	
	EA = 1; // hab todas as interrupcoes

	EX0 = 1;
	IT0 = 1;
	EX1 = 1;
	IT1 = 1;
	
	Init_LCD();

	TR0 = 1 ; //dispara timer 0

	while(1){
		if (inc != 0){		//interrupcao timer
			X1++;
			updtCurr();
			inc = 0;
		}
		if (lajj != 0){		//interrupcao ext0
			updtDiff();
			updtLast();			
			updtSmall();
			updtBig();
			lajj = 0;
		}
		if (rstval != 0){ //interrupcao ext1
			rstval = 0;
			resetSist();
		}
		
	}
}


void ext0(void) interrupt 0 {
	lajj++;
}

void c51_tim1 (void) interrupt 1 {
	TL0 = 0x05;
	TH0 = 0x97;
	inc++;
} //end of c51_int1

void ext1(void) interrupt 2{
	rstval++;
}

void updtCurr(void){
	position(1,12);
	wrt_num1(X1/60);
	send_byte(',',DADO);
	wrt_num2(X1%60);
}

void updtLast(void){
	X2 = X1;
	position(1,0);
	wrt_num1(X2/60);
	send_byte(',',DADO);
	wrt_num2(X2%60);	
}

void updtDiff(void){
	X3 = X1 - X2;
	position(1,6);
	wrt_num1(X3/60);
	send_byte(',',DADO);
	wrt_num2(X3%60);
}

void updtBig(void){
	if (maxX3 < X3){
		maxX3 = X3;
		position(2,8);
		wrt_num1(maxX3/60);
		send_byte(',',DADO);
		wrt_num2(maxX3%60);				
	}
}
void updtSmall(void){
	if (minX3 > X3){
		minX3 = X3;
		position(2,2);
		wrt_num1(minX3/60);
		send_byte(',',DADO);
		wrt_num2(minX3%60);		
		
	}
}

void delay(void)
{
	unsigned char teste;
	for(teste = 0; teste < 80; teste++)
	{
	}
}

void send_byte(unsigned char byte, unsigned char cmd_msg)
{
	P1 = byte & 0xF0;
	RS = cmd_msg;
	EN = 1;
	EN = 0;

	P1 = (byte << 4) & 0xF0;
	RS = cmd_msg;
	EN = 1;
	EN = 0;
	delay();
}

void Init_LCD(void)
{
	send_byte(0x22,CMD);
	send_byte(0x28,CMD);	//0x28	 2 lines, 5x8
	send_byte(0x0F,CMD); //0x0F	 Display On, Cursor ON, Blinking Cursor ON
}

void position(unsigned char line, unsigned char col)
{
	if(line == 1)
	{
		send_byte(0x80+col,CMD);
	}else if(line == 2)
	{
		send_byte(0xC0+col,CMD);
	}
}

void wrt_num1(unsigned char number1)
{
		unsigned char dez,unid;
		dez = number1 / 10;
		unid = number1 % 10;
		if (dez != 0){ 
			send_byte(dez+48,DADO);
		}
		send_byte(unid+48,DADO);
}
void wrt_num2(unsigned char number2)
{
		unsigned char dez,unid;
		dez = number2 / 10;
		unid = number2 % 10;
		send_byte(dez+48,DADO);
		send_byte(unid+48,DADO);
}

void resetSist(void){
	X1 = 0;
	X2 = 0;
	X3 = 0;
	maxX3 = 0;
	minX3 = 999;
	
	position(1,12);
	send_byte(' ',DADO);
	send_byte(' ',DADO);
	send_byte(' ',DADO);
	send_byte(' ',DADO);
	
	position(1,6);
	send_byte(' ',DADO);
	send_byte(' ',DADO);
	send_byte(' ',DADO);
	send_byte(' ',DADO);
	
	position(1,0);
	send_byte(' ',DADO);
	send_byte(' ',DADO);
	send_byte(' ',DADO);
	send_byte(' ',DADO);
	
	position(2,2);
	send_byte(' ',DADO);
	send_byte(' ',DADO);
	send_byte(' ',DADO);
	send_byte(' ',DADO);
		
	position(2,8);
	send_byte(' ',DADO);
	send_byte(' ',DADO);
	send_byte(' ',DADO);	
	send_byte(' ',DADO);

}

//fim do codg