/*
 * dshot.c
 *
 *  Created on: Sep 28, 2023
 *      Author: hooke
 */

#include <dshot/dshot.h>

#define MHZ_TO_HZ(x) 			((x) * 1000000)
#define DSHOT1200_HZ     		MHZ_TO_HZ(24)
#define DSHOT600_HZ     		MHZ_TO_HZ(12)
#define DSHOT300_HZ     		MHZ_TO_HZ(6)
#define DSHOT150_HZ     		MHZ_TO_HZ(3)
#define MOTOR_BIT_0            	7
#define MOTOR_BIT_1            	14
#define MOTOR_BITLENGTH        	20
#define DSHOT_FRAME_SIZE       	16
#define DSHOT_DMA_BUFFER_SIZE   18 /* resolution + frame reset (2us) */
#define DSHOT_MIN_THROTTLE      48
#define DSHOT_MAX_THROTTLE     	2047
#define DSHOT_RANGE 			(DSHOT_MAX_THROTTLE - DSHOT_MIN_THROTTLE)

/* Forward Declarations */
void DShot_DMA_Callback(DMA_HandleTypeDef *hdma);
uint16_t DShot_Prepare_Packet(uint16_t value);
void DShot_Prepare_DMA_Buffer(uint32_t *motor_dmabuffer, uint16_t value);

void DShot_Init(DShot_Handle_TypeDef* HandleStruct, DShot_Type_Enum Type)
{
	if (HandleStruct == NULL)
	{
		// return DSHOT_ERROR
	}

	TIM_HandleTypeDef* tim = HandleStruct->Init.TIMInstance;
	switch (HandleStruct->Init.TIMChannel)
	{
		case TIM_CHANNEL_1:
			HandleStruct->DMAInstance = tim->hdma[TIM_DMA_ID_CC1];
			HandleStruct->DMACCR = (uint32_t)&tim->Instance->CCR1;
			HandleStruct->TIMCCR = TIM_DMA_CC1;
			break;
		case TIM_CHANNEL_2:
			HandleStruct->DMAInstance = tim->hdma[TIM_DMA_ID_CC2];
			HandleStruct->DMACCR = (uint32_t)&tim->Instance->CCR2;
			HandleStruct->TIMCCR = TIM_DMA_CC2;
			break;
		case TIM_CHANNEL_3:
			HandleStruct->DMAInstance = tim->hdma[TIM_DMA_ID_CC3];
			HandleStruct->DMACCR = (uint32_t)&tim->Instance->CCR3;
			HandleStruct->TIMCCR = TIM_DMA_CC3;
			break;
		case TIM_CHANNEL_4:
			HandleStruct->DMAInstance = tim->hdma[TIM_DMA_ID_CC4];
			HandleStruct->DMACCR = (uint32_t)&tim->Instance->CCR4;
			HandleStruct->TIMCCR = TIM_DMA_CC4;
			break;
	}

	HandleStruct->DMABuffer = (uint32_t)malloc(DSHOT_DMA_BUFFER_SIZE + sizeof(uint32_t));

	// Setup the frequency
	uint32_t freq = 0;
	switch (Type)
	{
		case(DSHOT600):
				freq = DSHOT600_HZ;
		break;
		case(DSHOT300):
				freq = DSHOT300_HZ;
		break;
		case(DSHOT1200):
				freq = DSHOT1200_HZ;
		break;
		case(DSHOT150):
		default:
			freq = DSHOT150_HZ;
	}

	// Calculate prescaler
	uint16_t prescaler = lrintf((float) SystemCoreClock / freq + 0.01f) - 1;
	__HAL_TIM_SET_PRESCALER(tim, prescaler);
	__HAL_TIM_SET_AUTORELOAD(tim, MOTOR_BITLENGTH);

	// Setup callback (TIM_DMA_ID_CCx depends on timer channel)
	HandleStruct->DMAInstance->XferCpltCallback = DShot_DMA_Callback;

	// Start PWM
	HAL_TIM_PWM_Start(tim, HandleStruct->Init.TIMChannel);
}

// __HAL_TIM_DISABLE_DMA is needed to eliminate the delay between different dshot signals
void DShot_DMA_Callback(DMA_HandleTypeDef *hdma)
{
	TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

	if (hdma == htim->hdma[TIM_DMA_ID_CC1])
	{
		__HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);
	}
	else if(hdma == htim->hdma[TIM_DMA_ID_CC2])
	{
		__HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC2);
	}
	else if(hdma == htim->hdma[TIM_DMA_ID_CC3])
	{
		__HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC3);
	}
	else if(hdma == htim->hdma[TIM_DMA_ID_CC4])
	{
		__HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC4);
	}
}

void DShot_Write(DShot_Handle_TypeDef* handle, uint16_t value)
{
	DShot_Prepare_DMA_Buffer((uint32_t*)handle->DMABuffer, value);
	HAL_DMA_Start_IT(handle->DMAInstance, handle->DMABuffer, handle->DMACCR, DSHOT_DMA_BUFFER_SIZE);
	__HAL_TIM_ENABLE_DMA(handle->Init.TIMInstance, handle->TIMCCR);
}

uint16_t DShot_Prepare_Packet(uint16_t value)
{
	uint16_t packet;
	bool dshot_telemetry = false;

	packet = (value << 1) | (dshot_telemetry ? 1 : 0);

	// compute checksum
	unsigned csum = 0;
	unsigned csum_data = packet;

	for(int i = 0; i < 3; i++)
	{
        csum ^=  csum_data; // xor data by nibbles
        csum_data >>= 4;
	}

	csum &= 0xf;
	packet = (packet << 4) | csum;

	return packet;
}

// Convert 16 bits packet to 16 pwm signal
void DShot_Prepare_DMA_Buffer(uint32_t* buffer, uint16_t value)
{
	uint16_t packet;
	packet = DShot_Prepare_Packet(value);

	for(int i = 0; i < 16; i++)
	{
		buffer[i] = (packet & 0x8000) ? MOTOR_BIT_1 : MOTOR_BIT_0;
		packet <<= 1;
	}

	buffer[16] = 0;
	buffer[17] = 0;
}
