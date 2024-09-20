/*
 *
 * @file KPD_interface.h
 * @author Kareem Hussein
 * @brief main header file contain public functions prototypes, types and macros for KPD
 * @date 23/08/2024      
 * @version 1.0
 *
 */

#ifndef HAL_KPD_INC_KPD_INTERFACE_H_
#define HAL_KPD_INC_KPD_INTERFACE_H_


#define KPD_NOT_PRESSED			0xFF

                       /*************** APIS PROTO TYPES ***************/

void KPD_voidInit    (void);
void KPD_voidGetValue(u8 *copy_pu8ReturnedValue);


#endif /* HAL_KPD_INC_KPD_INTERFACE_H_ */
