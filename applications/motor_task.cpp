#include "cmsis_os.h"
#include "io/can/can.hpp"
#include "motor/rm_motor/rm_motor.hpp"

sp::CAN can1(&hcan1);

sp::RM_Motor motorA(1, sp::RM_Motors::GM6020);

extern "C" void motor_task()
{
  osDelay(500);

  can1.config();
  can1.start();

  while (true) {
    // 固定给 GM6020 0.1 N·m
    // 这里只验证 CAN 控制，不使用 PID
    motorA.cmd(0.1f);

    motorA.write(can1.tx_data);
    can1.send(motorA.tx_id);

    osDelay(1);
  }
}

extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef * hcan)
{
  auto stamp_ms = osKernelSysTick();

  while (HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO0) > 0) {
    if (hcan == &hcan1) {
      can1.recv();

      if (can1.rx_id == motorA.rx_id) {
        motorA.read(can1.rx_data, stamp_ms);
      }
    }
  }
}