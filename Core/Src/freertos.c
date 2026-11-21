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
#include "CAN_Driver.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

extern CAN_HandleTypeDef hcan;
extern uint8_t CAN_Transmit_Data_32Mask[6][8];
extern CAN_TxHeaderTypeDef MsgHeader_32Mask[6];
extern uint8_t CAN_Transmit_Data_32Mask_T[6][8];
extern CAN_TxHeaderTypeDef MsgHeader_32Mask_T[6];


/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern TxDatas TxHead_Data ;
extern RxDatas RxHead_Data ;
/* USER CODE END Variables */
/* Definitions for CAN_TX */
osThreadId_t CAN_TXHandle;
const osThreadAttr_t CAN_TX_attributes = {
  .name = "CAN_TX",
  .stack_size = 128 * 4,
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
/* Definitions for Queue_TxDatas */
osMessageQueueId_t Queue_TxDatasHandle;
const osMessageQueueAttr_t Queue_TxDatas_attributes = {
  .name = "Queue_TxDatas"
};
/* Definitions for Queue_RxDatas */
osMessageQueueId_t Queue_RxDatasHandle;
const osMessageQueueAttr_t Queue_RxDatas_attributes = {
  .name = "Queue_RxDatas"
};
/* Definitions for OLED_Mutex */
osMutexId_t OLED_MutexHandle;
const osMutexAttr_t OLED_Mutex_attributes = {
  .name = "OLED_Mutex"
};
/* Definitions for TX_Flag */
osSemaphoreId_t TX_FlagHandle;
const osSemaphoreAttr_t TX_Flag_attributes = {
  .name = "TX_Flag"
};
/* Definitions for RX_Flag */
osSemaphoreId_t RX_FlagHandle;
const osSemaphoreAttr_t RX_Flag_attributes = {
  .name = "RX_Flag"
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

  /* Create the semaphores(s) */
  /* creation of TX_Flag */
  TX_FlagHandle = osSemaphoreNew(1, 0, &TX_Flag_attributes);

  /* creation of RX_Flag */
  RX_FlagHandle = osSemaphoreNew(1, 0, &RX_Flag_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of Queue_TxDatas */
  Queue_TxDatasHandle = osMessageQueueNew (3, sizeof(TxDatas), &Queue_TxDatas_attributes);

  /* creation of Queue_RxDatas */
  Queue_RxDatasHandle = osMessageQueueNew (3, sizeof(RxDatas), &Queue_RxDatas_attributes);

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
      for (int i = 0; i < 6; i++) {
       HAL_CAN_ActivateNotification(&hcan, CAN_IT_TX_MAILBOX_EMPTY);//开启通知
      HAL_CAN_Transmit(&MsgHeader_32Mask_T[i], CAN_Transmit_Data_32Mask_T[i]);
      //如果发送成功中断中释放二进制信号量TX_Flag,在这里判断然后将数据加入队列中。
      osSemaphoreAcquire(TX_FlagHandle, osWaitForever);//申请信号量
      //将数据加入队列
      TxHead_Data.TxHeader.StdId = MsgHeader_32Mask_T[i].StdId;
      TxHead_Data.TxHeader.ExtId = MsgHeader_32Mask_T[i].ExtId;
      TxHead_Data.TxHeader.IDE = MsgHeader_32Mask_T[i].IDE;
      TxHead_Data.TxHeader.RTR = MsgHeader_32Mask_T[i].RTR;
      TxHead_Data.TxHeader.DLC = MsgHeader_32Mask_T[i].DLC;
      TxHead_Data.TxHeader.TransmitGlobalTime = MsgHeader_32Mask_T[i].TransmitGlobalTime;
      for (uint8_t j = 0; j < 8; j++) {
        TxHead_Data.TxData[j] = CAN_Transmit_Data_32Mask[i][j];
      }
      osMessageQueuePut(Queue_TxDatasHandle,&TxHead_Data,0,0);//将数据加入队列


      osDelay(2500);//2秒发一次
    }
    osDelay(2000);//2秒发一次
 


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
  CAN_RxHeaderTypeDef RxHeader;
  uint8_t RxData[8];
  
  
  /* Infinite loop */
  for(;;)
  {//接收

    HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING ); 
    //接收中断在接收邮箱有CAN报文时进入，此时中断中释放信号量，才能在这里读取
     osSemaphoreAcquire(RX_FlagHandle, osWaitForever);//申请信号量
    HAL_CAN_Receive(&RxHeader, RxData);

      
      RxHead_Data.RxHeader.StdId = RxHeader.StdId;
      RxHead_Data.RxHeader.ExtId = RxHeader.ExtId;
      RxHead_Data.RxHeader.IDE = RxHeader.IDE;
      RxHead_Data.RxHeader.RTR = RxHeader.RTR;
      RxHead_Data.RxHeader.DLC = RxHeader.DLC;
     
      for (uint8_t j = 0; j < 8; j++) {
        RxHead_Data.RxData[j] = *(RxData+j);
      }
        //入队列
      osMessageQueuePut(Queue_RxDatasHandle,&RxHead_Data,0,0);//将数据加入队列
  

    osDelay(1000);//100ms接收一次
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
  TxDatas TxData = {0};
  RxDatas RxData = {0};

  /* Infinite loop */
  for(;;)
  {
    //发送队列
    
    osMessageQueueGet(Queue_TxDatasHandle,&TxData,0,0);//获取发送队列数据
    if(TxData.TxHeader.IDE == CAN_ID_STD){
      OLED_ClearArea(40, 0, 88, 16);
      OLED_Printf(0, 0, OLED_8X16, "TxID: %x" ,TxData.TxHeader.StdId);
    }else{
      
      OLED_Printf(0, 0, OLED_8X16, "TxID: %x" ,TxData.TxHeader.ExtId);
    }
    OLED_Printf(0, 16, OLED_8X16, " %x %x %x %x " ,TxData.TxData[0],TxData.TxData[1],TxData.TxData[2],TxData.TxData[3]);
    OLED_Update();
    //接收队列数据
    osMessageQueueGet(Queue_RxDatasHandle,&RxData,0,0);//获取发送队列数据
    if(RxData.RxHeader.IDE == CAN_ID_STD){
      OLED_ClearArea(40, 32, 88, 16);
      OLED_Printf(0, 32, OLED_8X16, "RxID: %x" ,RxData.RxHeader.StdId);
    }else{
      
      OLED_Printf(0, 32, OLED_8X16, "RxID: %x" ,RxData.RxHeader.ExtId);
    }
    OLED_Printf(0, 48, OLED_8X16, " %x %x %x %x " ,RxData.RxData[0],RxData.RxData[1],RxData.RxData[2],RxData.RxData[3]);
    OLED_Update();
    osDelay(500);//500ms显示一次
  }
  /* USER CODE END OLED_Shows_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

