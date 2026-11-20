#include "CAN_Driver.h"



/**
 *
 * @return 返回过滤器配置状态，成功为HAL_OK
 * @retval 配置过滤器，共14个
 */
HAL_StatusTypeDef HAL_CAN_Filter(void) {

  CAN_FilterTypeDef FilterStructure;

  //过滤器0的设置，目前全部接收
  FilterStructure.FilterBank = 0;//第0个过滤器 0-13

  //32位屏蔽模式
  FilterStructure.FilterMode =CAN_FILTERMODE_IDMASK;//列表模式还是屏蔽模式 目前屏蔽
  FilterStructure.FilterScale =CAN_FILTERSCALE_32BIT;//16位还是32位 目前32位

  //高位和低位分别是什么 扩展帧不要忘了|0x04
  FilterStructure.FilterIdHigh = 0x0000;
  FilterStructure.FilterIdLow =0x0000;
  FilterStructure.FilterMaskIdHigh =0x0000;
  FilterStructure.FilterMaskIdLow =0x0000;

  FilterStructure.FilterFIFOAssignment =CAN_FILTER_FIFO0;//指定两个接收邮箱中的一个 FIFO0

  //启动
  FilterStructure.FilterActivation = CAN_FILTER_ENABLE;
  //HAL_CAN_ConfigFilter(&hcan, &FilterStructure);//返回执行结果


/*
  //过滤器1的设置，目前全部接收
  FilterStructure.FilterBank = 1;//第1个过滤器 0-13

    //32位列表模式
  FilterStructure.FilterMode =CAN_FILTERMODE_IDLIST;//列表模式还是屏蔽模式
  FilterStructure.FilterScale =CAN_FILTERSCALE_32BIT;//16位还是32位 目前32位

  //高位和低位分别是什么 目前过滤0x123和0x12345678
  FilterStructure.FilterIdHigh = (0x123<<21)>>16;
  FilterStructure.FilterIdLow =0x123<<21;
  FilterStructure.FilterMaskIdHigh =((0x12345678<<3) |0x04)>>16;
  FilterStructure.FilterMaskIdLow =(0x12345678<<3) |0x04;

  FilterStructure.FilterFIFOAssignment =CAN_FILTER_FIFO0;//指定两个接收邮箱中的一个 FIFO0/1

  //启动
  FilterStructure.FilterActivation = CAN_FILTER_ENABLE;
*/
  return HAL_CAN_ConfigFilter(&hcan, &FilterStructure);//返回执行结果

}


/**
 *
 * @param TxHeader 发送报文帧头，包含STDID EXTID等基本信息
 * @param TxData 发送数据，最多8Byte
 * @return 返回发送邮箱，共三个邮箱 CAN_TX_MAILBOX0/1/2
 */
uint32_t HAL_CAN_Transmit(CAN_TxHeaderTypeDef* TxHeader,uint8_t* TxData) {
  //将数据发送出去
  uint32_t TxMailbox;

  //HAL_CAN_GetTxMailboxesFreeLevel(&hcan);
  HAL_CAN_AddTxMessage(&hcan, TxHeader, TxData, &TxMailbox);//发送出去
  return TxMailbox;//0x01邮箱0 0x02邮箱1 0x04邮箱2 CAN_TX_MAILBOX0/1/2

}


/**
 *
 * @param RxHeader 接收报文帧头，包含STDID EXTID等基本信息
 * @param RxData 接收数据，最多8Byte
 * @return 返回接收状态
 */
HAL_StatusTypeDef HAL_CAN_Receive(CAN_RxHeaderTypeDef* RxHeader,uint8_t* RxData) {//FIFO0

  //从过滤器0中取帧
  if (HAL_CAN_GetRxFifoFillLevel(&hcan,CAN_RX_FIFO0) != 0)//获取指定接收 FIFO 中待读取报文的数量
  {
    return HAL_CAN_GetRxMessage(&hcan,CAN_RX_FIFO0, RxHeader,RxData);//从指定接收 FIFO 读取最早的一帧报文
  }

}



/* 任务的流程
  //初始化CAN
  HAL_CAN_Init(&hcan);

  HAL_CAN_ActivateNotification(
    &hcan,
    CAN_IT_RX_FIFO0_MSG_PENDING
);//开启FIFO0接收中断通知

  HAL_CAN_Filter();//过滤器0的配置
  HAL_CAN_Start(&hcan);//启动CAN外设
  */




  //后续目标， 实现CAN的同时发送和接收，利用freertos，先不考虑遥控帧
  //后续实现ISOTP，先实现单帧通信。