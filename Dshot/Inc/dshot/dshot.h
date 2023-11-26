/*
 * Dshot2.h
 *
 *  Created on: Oct 14, 2023
 *      Author: hooke
 */

#ifndef DSHOT_H_
#define DSHOT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "tim.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/**
  * @brief  DShot protocol enumeration
  * @note   Provides the various types of DShot protocols
  *
  */
typedef enum
{
	DSHOT150,
	DSHOT300,
	DSHOT600,
	DSHOT1200
} DShot_Type_Enum;

typedef struct
{
	TIM_HandleTypeDef* TIMInstance;
	uint16_t TIMChannel;

} DShot_Init_TypeDef;

/**
  * @brief  DShot Structure definition
  * @note   Structure used to configure the motor definition
  *
  */
typedef struct
{
	DShot_Init_TypeDef Init;
	DMA_HandleTypeDef* DMAInstance;
	uint32_t DMABuffer;
	uint32_t DMACCR;
	uint32_t TIMCCR;
	uint16_t Value;
} DShot_Handle_TypeDef;

void DShot_Init(DShot_Handle_TypeDef* InitStruct, DShot_Type_Enum Type);
void DShot_Release(DShot_Handle_TypeDef* InitStruct);
void DShot_Write(DShot_Handle_TypeDef* handle, uint16_t value);
void DShot_Prepare_DMA_Buffer(uint32_t* buffer, uint16_t value);
void DShot_Write_All(DShot_Handle_TypeDef* handle, int count);

#ifdef __cplusplus
}
#endif
#endif /* DSHOT_H_ */
