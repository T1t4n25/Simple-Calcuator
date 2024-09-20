/*
 *
 * @file KPD_program.c
 * @author Kareem Hussein
 * @brief main source for KPD functions implementation
 * @date 23/08/2024      
 * @version 1.0
 *
 */


/* UTILES_LIB */
#include "../Libs/STD_TYPES.h"
#include "../Libs/BIT_MATH.h"

/* MCAL */
#include "../MCAL/DIO_interface.h"

/* HAL */
#include "KPD_interface.h"
#include "KPD_config.h"


/*
 * @fn KPD_voidInit
 * @brief the function initializes the KPD ROWS pins as I/P with PullUp and COLS pins as O/P initialy high
 * @param[in] void
 * @retVal void
 */
void KPD_voidInit(void)
{
	/* Config ROWS pins as I/P pins */ 
	DIO_voidSetPinDirection(KPD_ROW0_PORT, KPD_ROW0_PIN, DIO_PIN_INPUT);
	DIO_voidSetPinDirection(KPD_ROW1_PORT, KPD_ROW1_PIN, DIO_PIN_INPUT);
	DIO_voidSetPinDirection(KPD_ROW2_PORT, KPD_ROW2_PIN, DIO_PIN_INPUT);
	DIO_voidSetPinDirection(KPD_ROW3_PORT, KPD_ROW3_PIN, DIO_PIN_INPUT);

	/* Activate Internal pull up for ROWS pins */ 
	DIO_voidActivePinInPullUpResistance(KPD_ROW0_PORT, KPD_ROW0_PIN);
	DIO_voidActivePinInPullUpResistance(KPD_ROW1_PORT, KPD_ROW1_PIN);
	DIO_voidActivePinInPullUpResistance(KPD_ROW2_PORT, KPD_ROW2_PIN);
	DIO_voidActivePinInPullUpResistance(KPD_ROW3_PORT, KPD_ROW3_PIN);

	/* Config COLS pins as O/P pins */
	DIO_voidSetPinDirection(KPD_COL0_PORT, KPD_COL0_PIN, DIO_PIN_OUTPUT);
	DIO_voidSetPinDirection(KPD_COL1_PORT, KPD_COL1_PIN, DIO_PIN_OUTPUT);
	DIO_voidSetPinDirection(KPD_COL2_PORT, KPD_COL2_PIN, DIO_PIN_OUTPUT);
	DIO_voidSetPinDirection(KPD_COL3_PORT, KPD_COL3_PIN, DIO_PIN_OUTPUT);

	/* Set COLS pins as Initially high */ 
	DIO_voidSetPinValue(KPD_COL0_PORT, KPD_COL0_PIN, DIO_PIN_HIGH);
	DIO_voidSetPinValue(KPD_COL1_PORT, KPD_COL1_PIN, DIO_PIN_HIGH);
	DIO_voidSetPinValue(KPD_COL2_PORT, KPD_COL2_PIN, DIO_PIN_HIGH);
	DIO_voidSetPinValue(KPD_COL3_PORT, KPD_COL3_PIN, DIO_PIN_HIGH);
}


/*
 * @fn KPD_voidGetValue
 * @brief the function Loop on COLS and activate each one, Loop on ROWS and check each one  
 * @param[out] copy_pu8ReturnedValue: The ASCII of pressed key if there is one.
 * @retVal void
 */
void KPD_voidGetValue(u8* copy_pu8ReturnedValue)
{
	if(copy_pu8ReturnedValue != NULL)
	{
		u8 local_u8ColsCounter, local_u8RowsCounter;
		u8 local_u8PinState;
		u8 local_u8ColsPorts[4] = {KPD_COL0_PORT, KPD_COL1_PORT, KPD_COL2_PORT, KPD_COL3_PORT};
		u8 local_u8ColsPins [4] = {KPD_COL0_PIN , KPD_COL1_PIN , KPD_COL2_PIN , KPD_COL3_PIN};
		u8 local_u8RowsPorts[4] = {KPD_ROW0_PORT, KPD_ROW1_PORT, KPD_ROW2_PORT, KPD_ROW3_PORT};
		u8 local_u8RowsPins [4] = {KPD_ROW0_PIN , KPD_ROW1_PIN , KPD_ROW2_PIN , KPD_ROW3_PIN};
		u8 local_u8KPD_Chars[4][4] = KPD_KEYS; 

		*copy_pu8ReturnedValue = KPD_NOT_PRESSED;
		for(local_u8ColsCounter = 0;local_u8ColsCounter < 4; local_u8ColsCounter++)
		{
			/* Activate Each Column with low value */
			DIO_voidSetPinValue(local_u8ColsPorts[local_u8ColsCounter], local_u8ColsPins[local_u8ColsCounter], DIO_PIN_LOW);

			for(local_u8RowsCounter = 0; local_u8RowsCounter < 4; local_u8RowsCounter++)
			{
				/* Check Each Row (if == 0?) */
				DIO_voidGetPinValue(local_u8RowsPorts[local_u8RowsCounter], local_u8RowsPins[local_u8RowsCounter], &local_u8PinState);

				/* Is Pressed? */
				if(0 == local_u8PinState)
				{
					while(0 == local_u8PinState)
					{
						DIO_voidGetPinValue(local_u8RowsPorts[local_u8RowsCounter], local_u8RowsPins[local_u8RowsCounter], &local_u8PinState);
					}
					*copy_pu8ReturnedValue = local_u8KPD_Chars[local_u8RowsCounter][local_u8ColsCounter];
					break;
				}
			}

			/* Deactivate Current Column with high value */
			DIO_voidSetPinValue(local_u8ColsPorts[local_u8ColsCounter], local_u8ColsPins[local_u8ColsCounter], DIO_PIN_HIGH);

			if(local_u8RowsCounter < 4)
			{
				break;
			}
		}
	}
	else
	{
		// return Error State
	}
}

