/*
 * ESP32.h
 *
 *  Created on: 16. 11. 2019
 *      Author: Marek
 */

#ifndef ESP32_H_
#define ESP32_H_

#include "stm32f0xx_hal.h"
#include <stdbool.h>

UART_HandleTypeDef huart2;
I2C_HandleTypeDef hi2c1;

bool Success;
bool BadConnection;
bool SyncRequired;
#define SSID "WiFi_SSID"
#define Password "WiFi_password"




void ESP32Send(uint8_t *SendData);

void WiFiConnect(void);
void ESPSyncToRTC(void);
void CleanBuffer(void);


#endif /* ESP32_H_ */
