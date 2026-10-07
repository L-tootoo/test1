#include "cmsis_os.h"
#include "io/plotter/plotter.hpp"
#include "motor/rm_motor/rm_motor.hpp"

extern sp::RM_Motor motorA;

sp::Plotter plotter(&huart1, false);

extern "C" void Print_Task(void * argument)
{
  while (1) {
    plotter.plot(motorA.angle, motorA.speed, motorA.torque);

    osDelay(10);
  }
}