/*
 *
 * @file LCD_interface.h
 * @author Kareem Hussein
 * @brief main header file contain public functions prototypes, types and macros for LCD
 * @date 17/08/2024      
 * @version 1.0
 *
 */

#ifndef LCD_INTERFACE_H_
#define LCD_INTERFACE_H_

/* Macros For LCD Shifting Direction */
#define LCD_SHIFT_LEFT          0
#define LCD_SHIFT_RIGHT         1

/* Macros For LCD Line Id */
#define LCD_LINE_ONE            0
#define LCD_LINE_TWO            1

                    /*************** APIS PROTOTYPES ***************/
					
void LCD_voidInit                (void);
void LCD_voidDisplayChar         (u8 copy_u8Data);
void LCD_voidSendCommand         (u8 copy_u8cmnd);
void LCD_voidDisplayString       (u8* copy_Pu8String);
void LCD_voidDisplayNumber       (s32 copy_u32Number);
void LCD_voidClear               (void);
void LCD_voidShift               (u8 copy_u8ShifttingDirection);
void LCD_voidGoToSpecificPosition(u8 copy_u8LineNumber, u8 copy_u8Position);


#endif /* LCD_INTERFACE_H_ */
