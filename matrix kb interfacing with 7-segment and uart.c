/*This program detects the key press in 4x4 matrix keyboard and display 
the key value in 7-segment display as well as in UART*/
#include<reg51.h>

sbit ones_sel=P2^0;
sbit tens_sel=P2^1;

unsigned char c,ones,tens;
unsigned char a[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7c,0x07,0x7f,0x67};
void delay(unsigned int x)
	{
		while(x--);
	}
void scan()
	{
		P1=0xfe;
		if(P1==0xee)
		c=0;
		if(P1==0xde)
		c=1;
		if(P1==0xbe)
		c=2;
		if(P1==0x7e)
		c=3;

		P1=0xfd;
		if(P1==0xed)
		c=4;
		if(P1==0xdd)
		c=5;
		if(P1==0xbd)
		c=6;
		if(P1==0x7d)
		c=7;

		P1=0xfb;
		if(P1==0xeb)
		c=8;
		if(P1==0xdb)
		c=9;
		if(P1==0xbb)
		c=10;
		if(P1==0x7b)
		c=11;

		P1=0xf7;
		if(P1==0xe7)
		c=12;
		if(P1==0xd7)
		c=13;
		if(P1==0xb7)
		c=14;
		if(P1==0x77)
		c=15;
	}

void binarytobcd()
	{
		tens=c/10;
		ones=c%10;	
	}
void serial()
	{
		SBUF='K';
		while(!TI);
		TI=0;
		SBUF='E';
		while(!TI);
		TI=0;
		SBUF='Y';
		while(!TI);
		TI=0;
		SBUF='=';
		while(!TI);
		TI=0;

		SBUF=tens|0x30;
		while(!TI);
		TI=0;
		SBUF=ones|0x30;
		while(!TI);
		TI=0;
		SBUF=0X0D;
		while(!TI);
		TI=0;
		SBUF=0X0A;
		while(!TI);
		TI=0;
		
	}
void main()
	{
		IE=0X82;
		SCON=0X50;
		TMOD=0X20;
		TH1=0XFD;
		TR1=1;
		TR0=1;
		P0=0x00;
		while(1)
			{
				scan();
				binarytobcd();
				serial();
			}
	
	
	}

void display_refreshing_timer0() interrupt 1
	{
		ones_sel=1;
		P0=a[ones];			
		delay(300);
		ones_sel=0;

		tens_sel=1;
		P0=a[tens];			
		delay(300);
		tens_sel=0;
	}
