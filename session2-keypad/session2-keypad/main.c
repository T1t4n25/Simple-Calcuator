/*
 * session2-keypad.c
 *
 * Created: 16/09/2024 09:47:14 ص
 * Author : Meteor
 */ 
#define F_CPU 8000000UL

#include "../MCAL/DIO_interface.h"
#include "../HAL/KPD_interface.h"
#include "../HAL/LCD_interface.h"
#include "../Libs/BIT_MATH.h"
//#include <stdio.h>
#include <util/delay.h> 
#include <string.h>

#define MAX_ARRAY_SIZE 17

void voidDecoderCalc(u8 u8_input_string[], s32 *s32_output);

int main(void)
{
	
	KPD_voidInit();
	LCD_voidInit();
	
	_delay_ms(30);
	u8 keyPressed = KPD_NOT_PRESSED;
	u8 input[MAX_ARRAY_SIZE];
	s32 result = 0;
	//result[MAX_ARRAY_SIZE - 1] = '\0';
	u8 index = 0;
    while (1) 
    {
		
		KPD_voidGetValue(&keyPressed);
		switch(keyPressed){
			case KPD_NOT_PRESSED:
				break;
			case 'c':
				LCD_voidClear();
				index = 0;
				memset(input, '\0', MAX_ARRAY_SIZE);
				result = 0;
				//memset(result, '\0', MAX_ARRAY_SIZE);
				break;
			case '=':
				voidDecoderCalc(input, &result);
				LCD_voidGoToSpecificPosition(1, 0);
				LCD_voidDisplayString((u8 *)"                ");
				LCD_voidGoToSpecificPosition(1, 0);
				LCD_voidDisplayNumber(result);
				result = 0;
				//memset(result, '\0', MAX_ARRAY_SIZE);
				break;
			default:
				if((index < MAX_ARRAY_SIZE)){
					// inside the 16 digit bound
					input[index] = keyPressed;
					input[index + 1] = '\0';
					index++;
					LCD_voidGoToSpecificPosition(0, 0);
					LCD_voidDisplayString(input);
				}
				else{
					// out of bounds
				}
				break;
		}
		
    }
	
}

void voidDecoderCalc(u8 u8_input_string[], s32 *s32_output) {
	u8 counter = 0;
	s32 num = 0;
	s32 sum = 0;
	char current_operator = '+';
	
	// Handle the first character being a negative sign
	if (u8_input_string[0] == '-') {
		current_operator = '-';
		counter++;
	}
	
	// Loop through the input string
	while (u8_input_string[counter] != '\0') {
		num = 0;
		
		// Convert consecutive digits into a number
		while (u8_input_string[counter] >= '0' && u8_input_string[counter] <= '9') {
			num = num * 10 + (u8_input_string[counter] - '0');
			counter++;
		}
		
		// Apply the current operator
		switch (current_operator) {
			case '/':
				if (num != 0) {  // Prevent division by zero
					sum /= num;
					}
			break;
			case '*':
				sum *= num;
				break;
			case '+':
				sum += num;
				break;
			case '-':
				sum -= num;
				break;
			default:
			break;
		}

		// Move to the next operator
		current_operator = u8_input_string[counter];
		counter++;
	}

	// Write the result to the output
	*s32_output = sum;
}
