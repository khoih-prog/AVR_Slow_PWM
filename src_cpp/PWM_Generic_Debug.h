/****************************************************************************************************************************
  PWM_Generic_Debug.h
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

  Version: 1.0.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K.Hoang      27/09/2021 Initial coding for AVR-based boards  (UNO, Nano, Mega, 32U4, 16U4, etc. )
 *****************************************************************************************************************************/

#pragma once

#ifndef PWM_GENERIC_DEBUG_H
#define PWM_GENERIC_DEBUG_H

#ifdef PWM_GENERIC_DEBUG_PORT
  #define PWM_DBG_PORT      PWM_GENERIC_DEBUG_PORT
#else
  #define PWM_DBG_PORT      Serial
#endif

// Change _PWM_LOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

#ifndef _PWM_LOGLEVEL_
  #define _PWM_LOGLEVEL_       1
#endif

#define PWM_LOGERROR(x)         if(_PWM_LOGLEVEL_>0) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.println(x); }
#define PWM_LOGERROR0(x)        if(_PWM_LOGLEVEL_>0) { PWM_DBG_PORT.print(x); }
#define PWM_LOGERRORLN0(x)      if(_PWM_LOGLEVEL_>0) { PWM_DBG_PORT.println(x); }
#define PWM_LOGERROR1(x,y)      if(_PWM_LOGLEVEL_>0) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(y); }
#define PWM_LOGERROR2(x,y,z)    if(_PWM_LOGLEVEL_>0) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(y); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(z); }
#define PWM_LOGERROR3(x,y,z,w)  if(_PWM_LOGLEVEL_>0) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(y); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(z); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(w); }

#define PWM_LOGWARN(x)          if(_PWM_LOGLEVEL_>1) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.println(x); }
#define PWM_LOGWARN0(x)         if(_PWM_LOGLEVEL_>1) { PWM_DBG_PORT.print(x); }
#define PWM_LOGWARNLN0(x)       if(_PWM_LOGLEVEL_>1) { PWM_DBG_PORT.println(x); }
#define PWM_LOGWARN1(x,y)       if(_PWM_LOGLEVEL_>1) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(y); }
#define PWM_LOGWARN2(x,y,z)     if(_PWM_LOGLEVEL_>1) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(y); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(z); }
#define PWM_LOGWARN3(x,y,z,w)   if(_PWM_LOGLEVEL_>1) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(y); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(z); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(w); }

#define PWM_LOGINFO(x)          if(_PWM_LOGLEVEL_>2) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.println(x); }
#define PWM_LOGINFO0(x)         if(_PWM_LOGLEVEL_>2) { PWM_DBG_PORT.print(x); }
#define PWM_LOGINFOLN0(x)       if(_PWM_LOGLEVEL_>2) { PWM_DBG_PORT.println(x); }
#define PWM_LOGINFO1(x,y)       if(_PWM_LOGLEVEL_>2) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(y); }
#define PWM_LOGINFO2(x,y,z)     if(_PWM_LOGLEVEL_>2) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(y); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(z); }
#define PWM_LOGINFO3(x,y,z,w)   if(_PWM_LOGLEVEL_>2) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(y); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(z); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(w); }

#define PWM_LOGDEBUG(x)         if(_PWM_LOGLEVEL_>3) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.println(x); }
#define PWM_LOGDEBUG0(x)        if(_PWM_LOGLEVEL_>3) { PWM_DBG_PORT.print(x); }
#define PWM_LOGDEBUGLN0(x)      if(_PWM_LOGLEVEL_>3) { PWM_DBG_PORT.println(x); }
#define PWM_LOGDEBUG1(x,y)      if(_PWM_LOGLEVEL_>3) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(y); }
#define PWM_LOGDEBUG2(x,y,z)    if(_PWM_LOGLEVEL_>3) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(y); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(z); }
#define PWM_LOGDEBUG3(x,y,z,w)  if(_PWM_LOGLEVEL_>3) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(y); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(z); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(w); }

#endif    //PWM_GENERIC_DEBUG_H
