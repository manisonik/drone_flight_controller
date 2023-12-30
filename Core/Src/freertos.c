/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "myscpi.h"
#include "sensor.h"
#include "dshot/dshot.h"
#include "usbd_cdc_if.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
DShot_Handle_TypeDef DShot_HandleStruct[4];
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes =
{ .name = "defaultTask", .stack_size = 128 * 4, .priority =
		(osPriority_t) osPriorityNormal, };
/* Definitions for ProcessCmdTask */
osThreadId_t ProcessCmdTaskHandle;
const osThreadAttr_t ProcessCmdTask_attributes =
{ .name = "ProcessCmdTask", .stack_size = 128 * 4, .priority =
		(osPriority_t) osPriorityNormal, };

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void MY_DSHOT_Init(void);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartProcessCmdTask(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
	/* USER CODE BEGIN Init */
	MY_DSHOT_Init();
	MY_SCPI_Init();
	MY_ICM20948_Init();
	/* USER CODE END Init */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* creation of defaultTask */
	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL,
			&defaultTask_attributes);

	/* creation of ProcessCmdTask */
	ProcessCmdTaskHandle = osThreadNew(StartProcessCmdTask, NULL,
			&ProcessCmdTask_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
	/* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
	/* init code for USB_DEVICE */
	MX_USB_DEVICE_Init();
	/* USER CODE BEGIN StartDefaultTask */
	/* Infinite loop */
	for (;;)
	{
		//DShot_Write(&DShot_HandleStruct[0], m0);
		//DShot_Write(&DShot_HandleStruct[1], m1);
		//DShot_Write(&DShot_HandleStruct[2], m2);
		//DShot_Write(&DShot_HandleStruct[3], m3);
		osDelay(1);
	}
	/* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartProcessCmdTask */
/**
 * @brief Function implementing the ProcessCmdTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartProcessCmdTask */
void StartProcessCmdTask(void *argument)
{
	/* USER CODE BEGIN StartProcessCmdTask */
	uint8_t rxData[8];
	memset(rxData, 0, 8);

	/* Infinite loop */
	for (;;)
	{
		uint16_t bytesAvailable = CDC_GetRxBufferBytesAvailable_FS();
		if (bytesAvailable > 0)
		{
			uint16_t bytesToRead = bytesAvailable >= 8 ? 8 : bytesAvailable;
			if (CDC_ReadRxBuffer_FS(rxData, bytesToRead)
					== USB_CDC_RX_BUFFER_OK)
			{
				MY_SCPI_Receive(rxData, bytesToRead);
				//while (CDC_Transmit_FS(rxData, bytesToRead) == USBD_BUSY);
			}
		}
		osDelay(1);
	}
	/* USER CODE END StartProcessCmdTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void MY_DSHOT_Init(void)
{
	// MOTOR 1 (PA3) - TIM5 Channel 4, DMA1 Stream 3
	DShot_HandleStruct[0].Init.TIMInstance = &htim3;
	DShot_HandleStruct[0].Init.TIMChannel = TIM_CHANNEL_1;
	DShot_Init(&DShot_HandleStruct[0], DSHOT600);

	// MOTOR 2 (PA2) - TIM2 Channel 3, DMA1 Stream 1
	DShot_HandleStruct[1].Init.TIMInstance = &htim3;
	DShot_HandleStruct[1].Init.TIMChannel = TIM_CHANNEL_2;
	DShot_Init(&DShot_HandleStruct[1], DSHOT600);

	// MOTOR 3 (PA0) - TIM2 Channel 1, DMA1 Stream 5
	DShot_HandleStruct[2].Init.TIMInstance = &htim3;
	DShot_HandleStruct[2].Init.TIMChannel = TIM_CHANNEL_3;
	DShot_Init(&DShot_HandleStruct[2], DSHOT600);

	// MOTOR 4 (PA1) - TIM5 Channel 2, DMA1 Stream 4
	DShot_HandleStruct[3].Init.TIMInstance = &htim3;
	DShot_HandleStruct[3].Init.TIMChannel = TIM_CHANNEL_4;
	DShot_Init(&DShot_HandleStruct[3], DSHOT600);
}
/* USER CODE END Application */

