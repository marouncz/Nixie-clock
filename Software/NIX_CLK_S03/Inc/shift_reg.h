/*
 * shift_reg.h
 *
 *  Created on: 3. 4. 2019
 *      Author: Marek
 */

#ifndef SHIFT_REG_H_
#define SHIFT_REG_H_

#include "stm32f0xx_hal.h"

SPI_HandleTypeDef hspi2;


void display(uint8_t num1, uint8_t num2, uint8_t num3, uint8_t num4, uint8_t num5, uint8_t num6);
void NixieClean(void);
#endif /* SHIFT_REG_H_ */
