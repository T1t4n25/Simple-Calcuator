/*
 *
 * @file LCD_program.c
 * @author Kareem Hussein
 * @brief main source for LCD functions implementation
 * @date 17/08/2024      
 * @version 1.0
 *
 */


#define F_CPU 8000000UL
#include <util/delay.h>

/* UTILES_LIB */
#include "STD_TYPES.h"
#include "BIT_MATH.h"

/* MCAL */
#include "DIO_interface.h"

/* HAL */
#include "LCD_interface.h"
#include "LCD_config.h"
#include "LCD_private.h"


void LCD_voidInit(void)
{
	// Intialize LCD pins As OutPut Pins
	DIO_voidSetPinDirection(LCD_RS_PORT, LCD_RS_PIN, DIO_PIN_OUTPUT);
	DIO_voidSetPinDirection(LCD_RW_PORT, LCD_RW_PIN, DIO_PIN_OUTPUT);
	DIO_voidSetPinDirection(LCD_E_PORT , LCD_E_PIN , DIO_PIN_OUTPUT);
	DIO_voidSetPinDirection(LCD_D4_PORT, LCD_D4_PIN, DIO_PIN_OUTPUT);
	DIO_voidSetPinDirection(LCD_D5_PORT, LCD_D5_PIN, DIO_PIN_OUTPUT);
	DIO_voidSetPinDirection(LCD_D6_PORT, LCD_D6_PIN, DIO_PIN_OUTPUT);
	DIO_voidSetPinDirection(LCD_D7_PORT, LCD_D7_PIN, DIO_PIN_OUTPUT);

	_delay_ms(35);

	/* Function Set (2Lines, 5*7 font)*/
	// set Rs pin = 0 (command)
	DIO_voidSetPinValue(LCD_RS_PORT, LCD_RS_PIN, DIO_PIN_LOW);
	// set RW pin = 0 (write)
	DIO_voidSetPinValue(LCD_RW_PORT, LCD_RW_PIN, DIO_PIN_LOW);
	PRV_voidWriteHalfPort(0b0010);
	PRV_voidEnableTriggerWrite();
	LCD_voidSendCommand(0b00101000);
	_delay_us(45);

	/* Display On/Off Control (DisplayOn, Cursor on, Blink on) */
	LCD_voidSendCommand(0b00001111);
	_delay_us(45);

	/* Display Clear */
	LCD_voidSendCommand(0b00000001);
	_delay_ms(2);

	/* Entery Mode Set (Increment on, Shift off) */
	LCD_voidSendCommand(0b00000110);
}


void LCD_voidDisplayChar(u8 copy_u8Data)
{
	// set Rs pin = 1 (data)
	DIO_voidSetPinValue(LCD_RS_PORT, LCD_RS_PIN, DIO_PIN_HIGH);

	// set RW pin = 0 (write)
	DIO_voidSetPinValue(LCD_RW_PORT, LCD_RW_PIN, DIO_PIN_LOW);

	// Write The Most 4 bits Of data on Data Pins
	PRV_voidWriteHalfPort(copy_u8Data>>4);

	/* Enable */
	PRV_voidEnableTriggerWrite();

	// Write The Least 4 bits Of data on Data Pins
	PRV_voidWriteHalfPort(copy_u8Data);

	/* Enable */
	PRV_voidEnableTriggerWrite();
}


void LCD_voidSendCommand(u8 copy_u8cmnd)
{
	// set Rs pin = 0 (command)
	DIO_voidSetPinValue(LCD_RS_PORT, LCD_RS_PIN, DIO_PIN_LOW);

	// set RW pin = 0 (write)
	DIO_voidSetPinValue(LCD_RW_PORT, LCD_RW_PIN, DIO_PIN_LOW);

	// Write The Most 4 bits Of command on Data Pins
	PRV_voidWriteHalfPort(copy_u8cmnd>>4);

	/* Enable */
	PRV_voidEnableTriggerWrite();

	// Write The Least 4 bits Of command on Data Pins
	PRV_voidWriteHalfPort(copy_u8cmnd);

	/* Enable */
	PRV_voidEnableTriggerWrite();
}


void LCD_voidDisplayString(u8* copy_Pu8String)
{
	if(copy_Pu8String != NULL)
	{
		u8 local_u8Counter = 0;
		while(copy_Pu8String[local_u8Counter] != '\0')
		{
			LCD_voidDisplayChar(copy_Pu8String[local_u8Counter]);
			local_u8Counter++;
		}
	}
	else
	{
		// return Error State
	}
}


void LCD_voidClear(void)
{
	/* Display Clear */
	LCD_voidSendCommand(0b00000001);
	_delay_ms(2);
}


void LCD_voidShift(u8 copy_u8ShifttingDirection)
{
	switch(copy_u8ShifttingDirection)
	{
	case LCD_SHIFT_LEFT:
		LCD_voidSendCommand(0b00011000);
		_delay_us(45);
		break;

	case LCD_SHIFT_RIGHT:
		LCD_voidSendCommand(0b00011100);
		_delay_us(45);
		break;
	}
}
void LCD_voidDisplayNumber(s32 copy_s32Number)
{
	u32 local_u32ReversedNumber = 1;
	u8 isNegative = 0;

	// Check if the number is negative
	if (copy_s32Number < 0)
	{
		isNegative = 1;  // Mark that the number is negative
		copy_s32Number = -copy_s32Number;  // Convert to positive for processing
	}

	// Special case: if the number is 0, display '0'
	if(copy_s32Number == 0)
	{
		LCD_voidDisplayChar('0');
	}
	else
	{
		// If the number is negative, display the '-' sign first
		if (isNegative)
		{
			LCD_voidDisplayChar('-');
		}

		// Reverse the number for correct display
		while(copy_s32Number != 0)
		{
			local_u32ReversedNumber = (local_u32ReversedNumber * 10) + (copy_s32Number % 10);
			copy_s32Number /= 10;
		}
		do
		{
			LCD_voidDisplayChar((local_u32ReversedNumber % 10) + '0');
			local_u32ReversedNumber /= 10;
		} while (local_u32ReversedNumber != 1);
	}
}


void LCD_voidGoToSpecificPosition(u8 copy_u8LineNumber, u8 copy_u8Position)
{
	switch(copy_u8LineNumber)
	{
	case LCD_LINE_ONE:
		LCD_voidSendCommand(0b10000000+copy_u8Position); //0x80+copy_u8Position
		_delay_us(45);
		break;

	case LCD_LINE_TWO:
		LCD_voidSendCommand(0b11000000+copy_u8Position); //0xC0+copy_u8Position
		_delay_us(45);
		break;
	}
}


static void PRV_voidWriteHalfPort(u8 copy_u8Value)
{
	if(0 == GET_BIT(copy_u8Value,0))
	{
		DIO_voidSetPinValue(LCD_D4_PORT, LCD_D4_PIN, DIO_PIN_LOW);
	}
	else
	{
		DIO_voidSetPinValue(LCD_D4_PORT, LCD_D4_PIN, DIO_PIN_HIGH);
	}
	if(0 == GET_BIT(copy_u8Value,1))
	{
		DIO_voidSetPinValue(LCD_D5_PORT, LCD_D5_PIN, DIO_PIN_LOW);
	}                                         
	else                                      
	{                                         
		DIO_voidSetPinValue(LCD_D5_PORT, LCD_D5_PIN, DIO_PIN_HIGH);
	}
	if(0 == GET_BIT(copy_u8Value,2))
	{
		DIO_voidSetPinValue(LCD_D6_PORT, LCD_D6_PIN, DIO_PIN_LOW);
	}                                         
	else                                      
	{                                         
		DIO_voidSetPinValue(LCD_D6_PORT, LCD_D6_PIN, DIO_PIN_HIGH);
	}
	if(0 == GET_BIT(copy_u8Value,3))
	{
		DIO_voidSetPinValue(LCD_D7_PORT, LCD_D7_PIN, DIO_PIN_LOW);
	}                                         
	else                                      
	{                                         
		DIO_voidSetPinValue(LCD_D7_PORT, LCD_D7_PIN, DIO_PIN_HIGH);
	}
	/*
     DIO_voidSetPinValue(LCD_D4_PORT, LCD_D4_PIN, GET_BIT(copy_u8Value,0));	
	 DIO_voidSetPinValue(LCD_D5_PORT, LCD_D5_PIN, GET_BIT(copy_u8Value,1));	
	 DIO_voidSetPinValue(LCD_D6_PORT, LCD_D6_PIN, GET_BIT(copy_u8Value,2));	
	 DIO_voidSetPinValue(LCD_D7_PORT, LCD_D6_PIN, GET_BIT(copy_u8Value,3));	
	 */
}


static void PRV_voidEnableTriggerWrite(void)
{
	/* Enable Pulse *//* H => L */
	DIO_voidSetPinValue(LCD_E_PORT, LCD_E_PIN, DIO_PIN_HIGH);
	_delay_ms(1);
	DIO_voidSetPinValue(LCD_E_PORT, LCD_E_PIN, DIO_PIN_LOW);
	_delay_ms(1);
}

