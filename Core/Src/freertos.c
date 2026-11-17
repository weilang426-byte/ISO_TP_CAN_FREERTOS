/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "OLED.h"
#include <stdint.h>
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

/* USER CODE END Variables */
/* Definitions for CAN_TX */
osThreadId_t CAN_TXHandle;
const osThreadAttr_t CAN_TX_attributes = {
  .name = "CAN_TX",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for CAN_RX */
osThreadId_t CAN_RXHandle;
const osThreadAttr_t CAN_RX_attributes = {
  .name = "CAN_RX",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for OLED_Shows */
osThreadId_t OLED_ShowsHandle;
const osThreadAttr_t OLED_Shows_attributes = {
  .name = "OLED_Shows",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for OLED_Mutex */
osMutexId_t OLED_MutexHandle;
const osMutexAttr_t OLED_Mutex_attributes = {
  .name = "OLED_Mutex"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void CAN_P_TX_Task(void *argument);
void CAN_P_RX_Task(void *argument);
void OLED_Shows_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of OLED_Mutex */
  OLED_MutexHandle = osMutexNew(&OLED_Mutex_attributes);

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
  /* creation of CAN_TX */
  CAN_TXHandle = osThreadNew(CAN_P_TX_Task, NULL, &CAN_TX_attributes);

  /* creation of CAN_RX */
  CAN_RXHandle = osThreadNew(CAN_P_RX_Task, NULL, &CAN_RX_attributes);

  /* creation of OLED_Shows */
  OLED_ShowsHandle = osThreadNew(OLED_Shows_Task, NULL, &OLED_Shows_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_CAN_P_TX_Task */
/**
  * @brief  Function implementing the CAN_TX thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_CAN_P_TX_Task */
void CAN_P_TX_Task(void *argument)
{
  /* USER CODE BEGIN CAN_P_TX_Task */
  /* Infinite loop */
  for(;;)
  {
    osMutexAcquire(OLED_MutexHandle, osWaitForever);
    for(uint8_t i = 0; i < 2; i++)
    {
       
    }
     OLED_Printf(0,0,OLED_8X16,"testtest1");
    OLED_Update();  
    osMutexRelease(OLED_MutexHandle);
    osDelay(2000);
  }
  /* USER CODE END CAN_P_TX_Task */
}

/* USER CODE BEGIN Header_CAN_P_RX_Task */
/**
* @brief Function implementing the CAN_RX thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CAN_P_RX_Task */
void CAN_P_RX_Task(void *argument)
{
  /* USER CODE BEGIN CAN_P_RX_Task */
  /* Infinite loop */
  for(;;)
  {
    osMutexAcquire(OLED_MutexHandle, osWaitForever);
    for(uint8_t i = 0; i < 2; i++)
    {
       
    }
    osMutexRelease(OLED_MutexHandle);
    osDelay(1000);
  }
  /* USER CODE END CAN_P_RX_Task */
}

/* USER CODE BEGIN Header_OLED_Shows_Task */
/**
* @brief Function implementing the OLED_Shows thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OLED_Shows_Task */
void OLED_Shows_Task(void *argument)
{
  /* USER CODE BEGIN OLED_Shows_Task */
  /* Infinite loop */
  for(;;)
  {
    osMutexAcquire(OLED_MutexHandle, osWaitForever);
    OLED_Printf(0,32,OLED_8X16,"testtest2");
    OLED_Update();
    osMutexRelease(OLED_MutexHandle);
    osDelay(1);
  }
  /* USER CODE END OLED_Shows_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

