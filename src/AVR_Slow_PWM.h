/****************************************************************************************************************************
  AVR_Slow_PWM.h
  For AVR-based boards  (UNO, Nano, Mega, 32U4, 16U4, etc. )
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/AVR_Slow_PWM
  Licensed under MIT license
  
  TCNTx - Timer/Counter Register. The actual timer value is stored here.
  OCRx - Output Compare Register
  ICRx - Input Capture Register (only for 16bit timer)
  TIMSKx - Timer/Counter Interrupt Mask Register. To enable/disable timer interrupts.
  TIFRx - Timer/Counter Interrupt Flag Register. Indicates a pending timer interrupt.

  Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
  unsigned long miliseconds), you just consume only one  RP2040-based timer and avoid conflicting with other cores' tasks.
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Version: 1.2.1

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K.Hoang      27/09/2021 Initial coding for AVR-based boards  (UNO, Nano, Mega, 32U4, 16U4, etc. )
  1.1.0   K Hoang      10/11/2021 Add functions to modify PWM settings on-the-fly
  1.2.0   K Hoang      29/01/2022 Fix multiple-definitions linker error. Improve accuracy
  1.2.1   K Hoang      30/01/2022 DutyCycle to be updated at the end current PWM period
*****************************************************************************************************************************/

#pragma once

#ifndef AVR_SLOW_PWM_H
#define AVR_SLOW_PWM_H

#include "AVR_Slow_PWM.hpp"
#include "AVR_Slow_PWM_Impl.h"

#include "AVR_Slow_PWM_ISR.h"


#endif    // AVR_SLOW_PWM_H

