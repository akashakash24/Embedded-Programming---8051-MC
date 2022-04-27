#include<reg51.h>
sbit ones_sel=P2^0;	 // Segment Selection bit(P2.0) for ones digit in 7-seg display
sbit tens_sel=P2^1;	 // Segment Selection bit(P2.1) for tens digit in 7-seg display
sbit hund_sel=P2^2;  // Segment Selection bit(P2.2) for hundreds digit in 7-seg display
												
// Generalized function for delay(Delay Routine)
void delay(unsigned int x)
	{
		while(x--);
	}

// One dimentional array for 7-segment code ( look-up-table)
unsigned char a[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7c,0x07,0x7f,0x67};

// variable declaration (8-bit character)
unsigned char ones,tens,hundreds,thousands,temp,c;

// Interrupt Service Routine(ISR) for entry (Ext. Int. 0) 
void ext0() interrupt 0
	{
		c=c+1; 			// Increment the count for every entry (falling edge)
		delay(1000);	// Wait for sometime (debouncing)
	}

// Interrupt Service Routine(ISR) for exit ( Ext. Int. 1)
void ext1() interrupt 2
	{
		if(c<=0)
		c=0;
		else
			{
				c=c-1;			// Decrement the count for every exit (falling edge)
				delay(1000);	// Wait for sometime (debouncing)
			}
	}

// Interrupt Service Routine(ISR) for display refreshing (8192 micro seconds) ( Timer 0)
void timer0() interrupt 1
	{
		ones_sel=1;	// Enabling ones segment
		P0=a[ones];	// Displaying ones in 7-segment display through Port 0 (after bcd to 7-seg code conversion through a[ones] (look-up-table))		
		delay(150); // wait for sometime (150 micro sec. approx.)
		ones_sel=0;	// Disabling ones segment

		tens_sel=1;	// Enabling tens segment
		P0=a[tens];	// Displaying tens in 7-segment display through Port 0 (after bcd to 7-seg code conversion through a[tens] (look-up-table))				
		delay(150);	// wait for sometime (150 micro sec. approx.)
		tens_sel=0;	// Disabling tens segment 

		hund_sel=1;		 // Enabling hundreds segment
		P0=a[hundreds];	 // Displaying hundreds in 7-segment display through Port (after bcd to 7-seg. code conversion through a[hundreds] (look-up-table))		
		delay(150);		 // wait for sometime (150 micro sec. approx.)
		hund_sel=0;		 // Disabling hundreds segment

	}

// Main Routine
void main()
	{
		//Interrupt Initialization
		IE=0x87; 	// Interrupt Initialization: Enabling Interrupts ( Ext Int.0, Ext Int.1, Timer0)
		IT0=1;		// Enable Ext. Int. 0 in Falling Edge triggered Mode
		IT1=1;		// Enable Ext. Int. 1 in Falling Edge Triggered Mode
		
		//Serial Port (UART) and Timer Initialization
		SCON=0X50;	// UART Initialization (Mode 1 with Receiver Enable)
		TMOD=0X20;	// Timer Initialization (Timer 1 in 8-bit Auto Reload Mode, Timer 0 is 13-bit timer Mode)
		TH1=0XFD;	// Timer 1 Higer Order byte Reload value for 9600 Baud
		TL1=0X00;	// Clearing TL1    		
		TR1=1;		// Start Timer 1 ( Timer 1 in 8-bit Auto Reload Mode for 9600 baud rate generation)
		TR0=1;		// Start Timer 0 ( Timer 0 in 13-bit Timer Mode for display refreshing ISR)
		while(1)
			{
				hundreds=c/100; // getting hundreds by dividing it by 100 (Quotient is hundreds)
				temp=c%100;		// Storing the remainder in temporary variable temp
				tens=temp/10;	// getting tens by dividing it by 10 ( Quotient of temp is tens)
				ones=temp%10;	// getting ones by taking modulus by 10 (Remainder of temp is ones)

				SBUF='C';   // Sending character C serially through SBUF
				while(!TI);	// Wait until all the bits shift out
				TI=0;		// Clear Transmit Interrupt (TI) Flag				
			
				SBUF='O';
				while(!TI);
				TI=0;						
			
				SBUF='U';
				while(!TI);
				TI=0;						
			
				SBUF='N';
				while(!TI);
				TI=0;						
			
				SBUF='T';
				while(!TI);
				TI=0;						
			
				SBUF=' ' ;
				while(!TI);
				TI=0;						
			
				SBUF='=';
				while(!TI);
				TI=0;						
			
				SBUF=' ';
				while(!TI);
				TI=0;						
			
				SBUF=hundreds|0x30;	// BCD to ASCII Code Conversion for hundreds by adding (logical OR) 0x30 and placing it in SBUF
				while(!TI);
				TI=0;						
			
				SBUF=tens|0x30;		// BCD to ASCII Code Conversion for tens by adding (logical OR) 0x30 and placing it in SBUF
				while(!TI);
				TI=0;						
			
				SBUF=ones|0x30;		// BCD to ASCII Code Conversion for ones by adding (logical OR) 0x30 and placing it in SBUF
				while(!TI);
				TI=0;						
			
				SBUF=0x0d; 			// Carriage Return(0x0d from ASCII Table to SBUF)
				while(!TI);
				TI=0;						
			
				SBUF=0x0a;			// New Line (0x0a from ASCII Table to SBUT)
				while(!TI);
				TI=0;
		
 		  	}
	  
	  }
				
