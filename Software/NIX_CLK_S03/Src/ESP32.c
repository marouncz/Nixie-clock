/*
 * ESP32.c
 *
 *  Created on: 16. 11. 2019
 *      Author: Marek
 */

#include "ESP32.h"

uint8_t HTTPRequest[] =
		"GET /api/timezone/Europe/Prague.txt HTTP/1.1\r\nHost: worldtimeapi.org\r\n\r\n";
uint16_t Rx_indx = 0;
uint8_t Rx_data[1], Rx_Buffer[700];

void ESP32Send(uint8_t *SendData)
{
	uint8_t SizeOfSendData = strlen(SendData);
	HAL_UART_Transmit_IT(&huart2, SendData, SizeOfSendData);

}

void WiFiConnect()
{
	HAL_UART_Receive_IT(&huart2, Rx_data, 1);
	CleanBuffer();
	HAL_Delay(3000);

	//ESP32Send("AT+RST\r\n");
	//HAL_Delay(5000);
	ESP32Send("ATE0\r\n");
	HAL_Delay(1000);
	ESP32Send("AT+CWQAP\r\n");
	HAL_Delay(500);
	ESP32Send("AT+CWMODE=1\r\n");
	HAL_Delay(3000);

	ESP32Send("AT+CWJAP=\"");
	HAL_Delay(1);
	ESP32Send(SSID);
	HAL_Delay(1);
	ESP32Send("\",\"");
	HAL_Delay(1);
	ESP32Send(Password);
	HAL_Delay(1);
	ESP32Send("\"\r\n");
	HAL_Delay(10000);
	ESP32Send("AT+CWHOSTNAME=\"Nixie Clock\"\r\n");
	HAL_Delay(1000);

}

void ESPSyncToRTC(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1);
	ESP32Send("AT+CIPSTART=\"TCP\",\"worldtimeapi.org\",80\r\n");
	HAL_Delay(500);
	CleanBuffer();
	ESP32Send("AT+CIPSEND=72\r\n");
	HAL_Delay(500);

	ESP32Send(HTTPRequest);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
	volatile uint16_t WaitTime = 0;
	BadConnection = 0;
	while ((Rx_indx < 300) && !BadConnection)
	{
		HAL_Delay(1);
		WaitTime++;
		if (WaitTime > 10000)
		{
			BadConnection = 1;

		}
	}
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, BadConnection);
	uint8_t SearchedString[8] = "datetime";
	volatile uint16_t AddressOfBeginning = 0;
	bool Found = 0;
	Success = 1;

	while (!Found && Success)
	{

		if ((SearchedString[0] == Rx_Buffer[AddressOfBeginning])
				& (SearchedString[1] == Rx_Buffer[(AddressOfBeginning + 1)])
				& (SearchedString[2] == Rx_Buffer[(AddressOfBeginning + 2)])
				& (SearchedString[3] == Rx_Buffer[(AddressOfBeginning + 3)])
				& (SearchedString[4] == Rx_Buffer[(AddressOfBeginning + 4)])
				& (SearchedString[5] == Rx_Buffer[(AddressOfBeginning + 5)])
				& (SearchedString[6] == Rx_Buffer[(AddressOfBeginning + 6)])
				& (SearchedString[7] == Rx_Buffer[(AddressOfBeginning + 7)]))
		{
			Found = 1;

		}
		else
		{
			AddressOfBeginning++;
			if ((AddressOfBeginning - 2) == sizeof(Rx_Buffer))
			{
				Success = 0;
			}
			//Found = 0;
		}

	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, !Success);
	if (Success)
	{
		uint8_t Time[6] =
		{ 0, (Rx_Buffer[AddressOfBeginning + 27] - 48) << 4
				| (Rx_Buffer[AddressOfBeginning + 28] - 48),
				(Rx_Buffer[AddressOfBeginning + 24] - 48) << 4
						| (Rx_Buffer[AddressOfBeginning + 25] - 48),
				(Rx_Buffer[AddressOfBeginning + 21] - 48) << 4
						| (Rx_Buffer[AddressOfBeginning + 22] - 48) };
		HAL_I2C_Master_Transmit(&hi2c1, 0x68 << 1, Time, 4, 50);
	}

	HAL_Delay(100);
	ESP32Send("AT+CIPCLOSE");
	HAL_Delay(100);
	CleanBuffer();

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	Rx_Buffer[Rx_indx] = Rx_data[0];
	Rx_indx++;
	HAL_UART_Receive_IT(&huart2, Rx_data, 1); //activate UART receive interrupt every time

}

void CleanBuffer(void)
{
	for (int i = 0; i < sizeof(Rx_Buffer); i++)
	{
		Rx_Buffer[i] = 0;
	}
	Rx_indx = 0;
}
