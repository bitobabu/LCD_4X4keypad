/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	BPager.c  
	Author:		BITO BABU
	Date:		22/01/2025
	Modified:	None
	� Fanshawe College, 2025

	Description: A device which can send and recieve messages for communication using RF transmission. 
=============================================================================*/

/* Preprocessor ===============================================================
   Hardware Configuration Bits ==============================================*/
#pragma config FOSC		= INTIO67
#pragma config PLLCFG	= OFF
#pragma config PRICLKEN = ON
#pragma config FCMEN	= OFF
#pragma config IESO		= OFF
#pragma config PWRTEN	= OFF 
#pragma config BOREN	= ON
#pragma config BORV		= 285 
#pragma config WDTEN	= OFF
#pragma config PBADEN	= OFF
#pragma config LVP		= OFF
#pragma config MCLRE	= EXTMCLR

// Libraries ==================================================================
#include <p18f45k22.h>
#include <stdio.h>
#include <stdlib.h>
#include <delays.h>




// Constants  =================================================================
#define TRUE	1	
#define FALSE	0
#define OSCMASK 0x52
#define T0FLAG	INTCONbits.TMR0IF









#define RS LATDbits.LATD1  // Register Select
#define EN LATDbits.LATD0  // Enable
#define D4 LATCbits.LATC0 // Data bit 4
#define D5 LATCbits.LATC1  // Data bit 5
#define D6 LATCbits.LATC2  // Data bit 6
#define D7 LATCbits.LATC3  // Data bit 7

#define ROW1 LATAbits.LATA0 // Row1 pin
#define ROW2 LATAbits.LATA1 // Row 2
#define ROW3 LATAbits.LATA2 // Row 3
#define ROW4 LATAbits.LATA3 // Row 4

#define COL1 PORTAbits.RA4 // Col 1
#define COL2 PORTAbits.RA5 // Col 2
#define COL3 PORTAbits.RA6 // Col 3
#define COL4 PORTAbits.RA7 // Col 4









// Global Variables ==========================================================




	char cmmnds[9][20]={"Rprt Fashion","Rprt Electrncs","Rprt Frnt Desk","Rprt Pharmcy","Rprt HBI","Rprt Produce","Cleaning  ","Carry out","Theft Noticed"};
	char names[8][8]={"Ann","Joseph","Dan","Lincy","Sush","Jordan","Pooja","Bito"};
	char index1=0;
	char index2=0;
	char buf[5]="Send";
	char *sampPtr=buf;
	char sendFlag=FALSE;


// Functions  =================================================================

/*>>> setOsc4Mhz: ===========================================================
Author:      BITO BABU
Date:        17/05/2024
Modified:    None
Desc:        The function sets the oscillator frequency to 4MHZ.
Input:       None 
Returns:     None 
 ============================================================================*/
void setOsc4Mhz()
{
    OSCCON=OSCMASK;
    while(!OSCCONbits.HFIOFS); 
} // eo setosc4MHZ()::

/*>>> configPorts: ===========================================================
Author:      BITO BABU
Date:        17/05/2024
Modified:    None
Desc:        The function configure all the ports.
Input:       None 
Returns:     None 
 ============================================================================*/
void configPorts()
{
    ANSELA =0x00;
	LATA   =0x0F;
	TRISA  =0xF0;
	
	ANSELC =0x00;
	LATC   =0x00;
	TRISC  =0xF0;

	ANSELD =0x00;
	LATD   =0x00;
	TRISD  =0xF8;

	ANSELB =0x00;
	LATB   =0x00;
	TRISB  =0xFF;

	ANSELE =0x00;
	LATE   =0x00;
	TRISE  =0xFF;

} //eo configPorts()::

/*>>> configTMR0: ===========================================================
Author:      BITO BABU
Date:        17/05/2024
Modified:    None
Desc:        The function configure the Timer0 module.
Input:       None 
Returns:     None 
 ============================================================================*/
void configTMR0()
{
	T0CON=0x93;
}

/*>>> resetTMR0: ===========================================================
Author:      BITO BABU
Date:        17/05/2024
Modified:    None
Desc:        The function set the prescalevalue and resets the Timer0.
Input:       None 
Returns:     None 
 ============================================================================*/
void resetTMR0()
{
	T0FLAG=FALSE;
	TMR0H=0x0B;
	TMR0L=0xDC;
}//eo resetTMR0()::







/*>>> systemInitialization: ===========================================================
Author:      BITO BABU
Date:        14/06/2024
Modified:    None
Desc:        This fucntion initialzes all other functions.
Input:       None 
Returns:     None 
 ============================================================================*/
void systemInitialization()
{
	setOsc4Mhz();
	configPorts();
		
}//eo  systemInitialization()::






/*>>> sndCmndLCD: ===========================================================
Author:      BITO BABU
Date:        4/02/2025
Modified:    None
Desc:        This fucntion gives commands to LCD screen.
Input:       commands in hexvalue 
Returns:     None 
 ============================================================================*/
void sndCmndLCD(char cmd)
{
	LATC = (LATC & 0xF0)|(cmd >> 4);
	RS=FALSE; //command Register is selected
	EN=TRUE; //High-to-low pulse on Enable pin to latch data
	_asm NOP _endasm// but is this delay enough
	EN=FALSE;
	Delay1KTCYx(10);
	LATC = (LATC & 0xF0)|(cmd & 0x0F);  /*Send lower nibble of command to PORT */
	EN = TRUE;
	_asm NOP _endasm
	EN = FALSE;
	Delay1KTCYx(10);
}



/*>>>sndDtaLCD: ===========================================================
Author:      BITO BABU
Date:        4/02/2025
Modified:    None
Desc:        This fucntion prints data to LCD screen.
Input:       Data
Returns:     None 
 ============================================================================*/
void sndDtaLCD(char data)
{
	LATC = (LATC & 0xF0)|(data >> 4);
	RS=TRUE; //Data Register is selected
	EN=TRUE; //High-to-low pulse on Enable pin to latch data
	_asm NOP _endasm
	EN=FALSE;
	Delay1KTCYx(5);
	LATC = (LATC & 0xF0)|(data & 0x0F);  /*Send lower nibble of command to PORT */
	EN = TRUE;
	_asm NOP _endasm
	EN = FALSE;
	Delay1KTCYx(5);
	
}

/*>>> displayLCD: ===========================================================
Author:      BITO BABU
Date:        4/02/2025
Modified:    None
Desc:        This fucntion prints string to LCD screen.
Input:       Data string
Returns:     None 
 ============================================================================*/

void displayLCD(char *msg)
{
	while((*msg)!=0)
	{		
	  sndDtaLCD(*msg);
	  msg++;	
    }
}//eo strdispLCD




/*>>> fdisplayLCD: ===========================================================
Author:      BITO BABU
Date:        4/02/2025
Modified:    None
Desc:        This fucntion prints string to LCD screen.
Input:       Data string
Returns:     None 
 ============================================================================*/

void fdisplayLCD(char row,char pos,char *msg)
{
	char location=0;
    if(row<=1)
    {
        location=(0x80) | (pos & 0x0f);  /*Print message on 1st row and desired location*/
        sndCmndLCD(location);
    }
    else
    {
        location=(0xC0) | (pos & 0x0f);  /*Print message on 2nd row and desired location*/
        sndCmndLCD(location);    
    }  
    
  	 displayLCD(msg);
}//eo fdisplayLCD



/*>>> clearLCD: ===========================================================
Author:      BITO BABU
Date:        4/02/2025
Modified:    None
Desc:        This fucntion clear the LCD screen.
Input:       None
Returns:     None 
 ============================================================================*/
void clearLCD()
{
    sndCmndLCD(0x01);  /*clear display screen*/
	Delay1KTCYx(10);
   
}
/*>>> initializeLCD: ===========================================================
Author:      BITO BABU
Date:        4/02/2025
Modified:    None
Desc:        This fucntion initialzes the LCD screen.
Input:       None 
Returns:     None 
 ============================================================================*/
void initializeLCD()
{
	TRISC=0xC0;
	Delay10KTCYx(1);  // Make 10 ms delay
	Delay1KTCYx(5);   //  Make 5 ms delay
	sndCmndLCD(0x02); // initialize LCD
	sndCmndLCD(0x28); //use 2 line and initialize 5*7 matrix in 4-bit mode
	sndCmndLCD(0x01);	//clear display screen
	sndCmndLCD(0x0C); //display on cursor off
	sndCmndLCD(0x06); //increment cursor 
	
}//eo  initializeLCD::

/*>>> scanKeypad: ===========================================================
Author:      BITO BABU
Date:        4/02/2025
Modified:    None
Desc:        This fucntion scan the keypad.
Input:       None 
Returns:     Key Pressed
 ============================================================================*/
void scanKeypad()
{
	ROW1=0;ROW2=ROW3=ROW4=1;
	if(!COL3)
	{
		Delay1KTCYx(5);
		index1=0;
		sndCmndLCD(0x01);
	}
	ROW2=0;ROW1=ROW3=ROW4=1;
	if(!COL1)
	{
		sndCmndLCD(0x01);
		fdisplayLCD(1,1,sampPtr);
		sendFlag=TRUE;		
	}
	if(!COL2)
	{
		Delay1KTCYx(5);
		index1=9;
		sndCmndLCD(0x01);
	}
	if(!COL3)
	{
		Delay1KTCYx(5);
		index1=8;
		sndCmndLCD(0x01);
	}
	if(!COL4)
	{
		Delay1KTCYx(5);
		index1=7;
		sndCmndLCD(0x01);
	}
	ROW3=0;ROW1=ROW2=ROW4=1;
	if(!COL2)
	{
		Delay1KTCYx(5);
		index1=6;
		sndCmndLCD(0x01);
	}
	if(!COL3)
	{
		Delay1KTCYx(5);
		index1=5;
		sndCmndLCD(0x01);
	}
	if(!COL4)
	{
		Delay1KTCYx(5);
		index1=4;
		sndCmndLCD(0x01);
	}
	ROW4=0;ROW2=ROW3=ROW1=1;
	if(!COL1)
	{
		Delay1KTCYx(5);
		index2++;
		if(index2==8)
		{
			index2=0;
		}
		sndCmndLCD(0x01);
	}
	if(!COL2)
	{
		Delay1KTCYx(5);
		index1=3;
		sndCmndLCD(0x01);
	}
	if(!COL3)
	{
		Delay1KTCYx(5);
		index1=2;
		sndCmndLCD(0x01);
	}
	if(!COL4)
	{
		Delay1KTCYx(5);
		index1=1;
		sndCmndLCD(0x01);
	}


}//eo  scanKeypad::


/*=== MAIN: FUNCTION ==========================================================
 ============================================================================*/
void main( void )
{
	char count=0;
	systemInitialization();
	initializeLCD();
	while(TRUE)
	{
		if(!sendFlag)
		{
			scanKeypad();
			
			fdisplayLCD(1,1,names[index2]);
			fdisplayLCD(2,1,cmmnds[index1]);
		}
		if(sendFlag)
		{
			if(T0FLAG)
			{
				resetTMR0();
				count++;
				if(count==2)
				{
					sendFlag=FALSE;
				}
				
			}
		
		}
	}
}


