#ifndef _CAN_DRIVER_H
#define _CAN_DRIVER_H

#include "stm32f1xx_hal.h"


extern CAN_HandleTypeDef hcan;



HAL_StatusTypeDef HAL_CAN_Filter(void);
//CAN有三个发送邮箱， TXheader box邮箱号
uint32_t HAL_CAN_Transmit(CAN_TxHeaderTypeDef* TxHeader,uint8_t* TxData);

//CAN有两个接收邮箱，每个有3个位置
HAL_StatusTypeDef HAL_CAN_Receive(CAN_RxHeaderTypeDef* RxHeader,uint8_t* RxData);



#endif /* _CAN_DRIVER_H */