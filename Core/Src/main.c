/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "system_init.h"
#include "cmsis_os.h"

#define MSGQUEUE_OBJECTS 16                     // number of Message Queue Objects

typedef struct {                                // object data type
  uint8_t Buf[32];
  uint8_t Idx;
} MSGQUEUE_OBJ_t;

osMessageQueueId_t mid_MsgQueue;                // message queue id

osThreadId_t tid_Thread_MsgQueue1;              // thread id 1
osThreadId_t tid_Thread_MsgQueue2;              // thread id 2

void Thread_MsgQueue1 (void *argument) {
  MSGQUEUE_OBJ_t msg;

  while (1) {
    ; // Insert thread code here...
    msg.Buf[0] = 0x55U;                                         // do some work...
    msg.Idx    = 0U;
    osMessageQueuePut(mid_MsgQueue, &msg, 0U, 1000U);
    osDelay(5000);                                            // suspend thread
  }
}

void Thread_MsgQueue2 (void *argument) {
  MSGQUEUE_OBJ_t msg;
  osStatus_t status;

  while (1) {
    ; // Insert thread code here...
    status = osMessageQueueGet(mid_MsgQueue, &msg, NULL, 4000U);   // wait for message
    if (status == osOK) {
      DebugPrint(&huart3, "Packet Received.");
    }
    else
    {
    	DebugPrint(&huart3, "Packet haven't received yet.");
    }
  }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system */
  MX_GPIO_Init();
  MX_USART3_UART_Init();

  /* Init scheduler */
  osKernelInitialize();

  mid_MsgQueue = osMessageQueueNew(MSGQUEUE_OBJECTS, sizeof(MSGQUEUE_OBJ_t), NULL);
  if (mid_MsgQueue == NULL) {
	  ; // Message Queue object not created, handle failure
  }

  tid_Thread_MsgQueue1 = osThreadNew(Thread_MsgQueue1, NULL, NULL);
  if (tid_Thread_MsgQueue1 == NULL) {
	  return(-1);
  }
  tid_Thread_MsgQueue2 = osThreadNew(Thread_MsgQueue2, NULL, NULL);
  if (tid_Thread_MsgQueue2 == NULL) {
	  return(-1);
  }

  osKernelStart();

}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
