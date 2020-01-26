/*
 * shift_reg.c
 *
 *  Created on: 3. 4. 2019
 *      Author: Marek
 */

#include "shift_reg.h"
#include <string.h>


uint8_t NixieCleanDelay = 50;

void display(uint8_t num1, uint8_t num2, uint8_t num3, uint8_t num4,
		uint8_t num5, uint8_t num6)
{
	uint8_t data[6] =
	{ 255 - num6, 255 - num5, 255 - num4, 255 - num3, 255 - num2, 255 - num1 };

	HAL_SPI_Transmit(&hspi2, data, 6, 100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
}

void NixieClean(void)
{

	for (int n = 0; n < 10; n++)
	{
		display(n, n, n, n, n, n);
		HAL_Delay(NixieCleanDelay);
	}
	for (int n = 8; n > 0; n--)
	{
		display(n, n, n, n, n, n);
		HAL_Delay(NixieCleanDelay);
	}

}
