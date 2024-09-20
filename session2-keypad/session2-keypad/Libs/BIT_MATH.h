/******************************************************************************
 *                                                                            *
 *************************  SWC     : BIT MATH        *************************
 *************************  Date    : 03/08/2024      *************************
 *************************  Author  : Kareem Hussein  *************************
 *************************  Version : 1.0             *************************
 *                                                                            *
 ******************************************************************************/


#ifndef BIT_MATH_H_
#define BIT_MATH_H_


#define SET_BIT(REG, BIT_NUM)			REG|=(1<<BIT_NUM)
#define CLR_BIT(REG, BIT_NUM)			REG&=(~(1<<BIT_NUM))
#define TOG_BIT(REG, BIT_NUM)			REG^=(1<<BIT_NUM)
#define GET_BIT(REG, BIT_NUM)           ((REG>>BIT_NUM)&0X01)


#endif