/****************************************************************************************************************************
  AVR_Slow_PWM_ISR.hpp
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

  Version: 1.1.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K.Hoang      27/09/2021 Initial coding for AVR-based boards  (UNO, Nano, Mega, 32U4, 16U4, etc. )
  1.1.0   K Hoang      10/11/2021 Add functions to modify PWM settings on-the-fly
*****************************************************************************************************************************/

#pragma once

#ifndef AVR_SLOW_PWM_ISR_HPP
#define AVR_SLOW_PWM_ISR_HPP

#include <string.h>

/////////////////////////////////////////////////// 


uint32_t timeNow()
{
#if USING_MICROS_RESOLUTION  
  return ( (uint32_t) micros() );
#else
  return ( (uint32_t) millis() );
#endif    
}
 
/////////////////////////////////////////////////// 

AVR_Slow_PWM_ISR::AVR_Slow_PWM_ISR()
  : numChannels (-1)
{
}

///////////////////////////////////////////////////

void AVR_Slow_PWM_ISR::init() 
{
  uint32_t currentTime = timeNow();
   
  for (uint8_t channelNum = 0; channelNum < MAX_NUMBER_CHANNELS; channelNum++) 
  {
    memset((void*) &PWM[channelNum], 0, sizeof (PWM_t));
    PWM[channelNum].prevTime = currentTime;
    PWM[channelNum].pin      = INVALID_AVR_PIN;
  }
  
  numChannels = 0;
}

///////////////////////////////////////////////////

void AVR_Slow_PWM_ISR::run() 
{    
  //uint32_t currentTime = timeNow();
  uint32_t currentTime;

  for (uint8_t channelNum = 0; channelNum < MAX_NUMBER_CHANNELS; channelNum++) 
  {
    currentTime = timeNow();
    
    // If enabled => check
    // start period / dutyCycle => digitalWrite HIGH
    // end dutyCycle =>  digitalWrite LOW
    if (PWM[channelNum].enabled) 
    {
      if ( (uint32_t) (currentTime - PWM[channelNum].prevTime) <= PWM[channelNum].onTime ) 
      {              
        if (!PWM[channelNum].pinHigh)
        {
          digitalWrite(PWM[channelNum].pin, HIGH);
          PWM[channelNum].pinHigh = true;
          
          // callbackStart
          if (PWM[channelNum].callbackStart != nullptr)
          {
            (*(timer_callback) PWM[channelNum].callbackStart)();
          }
        }
      }
      else if ( (uint32_t) (currentTime - PWM[channelNum].prevTime) < PWM[channelNum].period ) 
      {
        if (PWM[channelNum].pinHigh)
        {
          digitalWrite(PWM[channelNum].pin, LOW);
          PWM[channelNum].pinHigh = false;
          
          // callback when PWM pulse stops (LOW)
          if (PWM[channelNum].callbackStop != nullptr)
          {
            (*(timer_callback) PWM[channelNum].callbackStop)();
          }
        }
      }
      else 
      //else if ( (uint32_t) (currentTime - PWM[channelNum].prevTime) >= PWM[channelNum].period )   
      {
        PWM[channelNum].prevTime = currentTime;
      }      
    }
  }
}


///////////////////////////////////////////////////

// find the first available slot
// return -1 if none found
int AVR_Slow_PWM_ISR::findFirstFreeSlot() 
{
  // all slots are used
  if (numChannels >= MAX_NUMBER_CHANNELS) 
  {
    return -1;
  }

  // return the first slot with no callbackStart (i.e. free)
  for (uint8_t channelNum = 0; channelNum < MAX_NUMBER_CHANNELS; channelNum++) 
  {
    if ( (PWM[channelNum].period == 0) && !PWM[channelNum].enabled )
    {
      return channelNum;
    }
  }

  // no free slots found
  return -1;
}

///////////////////////////////////////////////////

int AVR_Slow_PWM_ISR::setupPWMChannel(uint32_t pin, uint32_t period, uint32_t dutycycle, void* cbStartFunc, void* cbStopFunc)
{
  int channelNum;
  
  // Invalid input, such as period = 0, etc
  if ( (period == 0) || (dutycycle > 100) )
  {
    PWM_LOGERROR(F("Error: Invalid period or dutycycle"));
    return -1;
  }

  if (numChannels < 0) 
  {
    init();
  }
 
  channelNum = findFirstFreeSlot();
  
  if (channelNum < 0) 
  {
    return -1;
  }

  PWM[channelNum].pin           = pin;
  PWM[channelNum].period        = period;
  PWM[channelNum].onTime        = ( period * dutycycle ) / 100;
  
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  PWM[channelNum].pinHigh       = true;
  
  PWM[channelNum].prevTime      = timeNow();
  
  PWM[channelNum].callbackStart = cbStartFunc;
  PWM[channelNum].callbackStop  = cbStopFunc;
  
  PWM_LOGINFO0(F("Channel : ")); PWM_LOGINFO0(channelNum); 
  PWM_LOGINFO0(F("\tPeriod : ")); PWM_LOGINFO0(PWM[channelNum].period);
  PWM_LOGINFO0(F("\t\tOnTime : ")); PWM_LOGINFO0(PWM[channelNum].onTime);
  PWM_LOGINFO0(F("\tStart_Time : ")); PWM_LOGINFOLN0(PWM[channelNum].prevTime);
 
  numChannels++;
  
  PWM[channelNum].enabled      = true;
  
  return channelNum;
}

///////////////////////////////////////////////////

bool AVR_Slow_PWM_ISR::modifyPWMChannel_Period(unsigned channelNum, uint32_t pin, uint32_t period, uint32_t dutycycle)
{
  // Invalid input, such as period = 0, etc
  if ( (period == 0) || (dutycycle > 100) )
  {
    PWM_LOGERROR("Error: Invalid period or dutycycle");
    return false;
  }

  if (channelNum > MAX_NUMBER_CHANNELS) 
  {
    PWM_LOGERROR("Error: channelNum > MAX_NUMBER_CHANNELS");
    return false;
  }
  
  if (PWM[channelNum].pin != pin) 
  {
    PWM_LOGERROR("Error: channelNum and pin mismatched");
    return false;
  }
   
  PWM[channelNum].period        = period;
  PWM[channelNum].onTime        = ( period * dutycycle ) / 100;
  
  digitalWrite(pin, HIGH);
  PWM[channelNum].pinHigh       = true;
  
  PWM[channelNum].prevTime      = timeNow();
     
  PWM_LOGINFO0("Channel : "); PWM_LOGINFO0(channelNum); PWM_LOGINFO0("\tPeriod : "); PWM_LOGINFO0(PWM[channelNum].period);
  PWM_LOGINFO0("\t\tOnTime : "); PWM_LOGINFO0(PWM[channelNum].onTime); PWM_LOGINFO0("\tStart_Time : "); PWM_LOGINFOLN0(PWM[channelNum].prevTime);
  
  return true;
}


///////////////////////////////////////////////////

void AVR_Slow_PWM_ISR::deleteChannel(unsigned channelNum) 
{
  if (channelNum >= MAX_NUMBER_CHANNELS) 
  {
    return;
  }

  // nothing to delete if no timers are in use
  if (numChannels == 0) 
  {
    return;
  }

  // don't decrease the number of timers if the specified slot is already empty (zero period, invalid)
  if ( (PWM[channelNum].pin != INVALID_AVR_PIN) && (PWM[channelNum].period != 0) )
  {
    memset((void*) &PWM[channelNum], 0, sizeof (PWM_t));
    
    PWM[channelNum].pin = INVALID_AVR_PIN;
    
    // update number of timers
    numChannels--;
  }
}

///////////////////////////////////////////////////

void AVR_Slow_PWM_ISR::restartChannel(unsigned channelNum) 
{
  if (channelNum >= MAX_NUMBER_CHANNELS) 
  {
    return;
  }

  PWM[channelNum].prevTime = timeNow();
}

///////////////////////////////////////////////////

bool AVR_Slow_PWM_ISR::isEnabled(unsigned channelNum) 
{
  if (channelNum >= MAX_NUMBER_CHANNELS) 
  {
    return false;
  }

  return PWM[channelNum].enabled;
}

///////////////////////////////////////////////////

void AVR_Slow_PWM_ISR::enable(unsigned channelNum) 
{
  if (channelNum >= MAX_NUMBER_CHANNELS) 
  {
    return;
  }

  PWM[channelNum].enabled = true;
}

///////////////////////////////////////////////////

void AVR_Slow_PWM_ISR::disable(unsigned channelNum) 
{
  if (channelNum >= MAX_NUMBER_CHANNELS) 
  {
    return;
  }

  PWM[channelNum].enabled = false;
}

///////////////////////////////////////////////////

void AVR_Slow_PWM_ISR::enableAll() 
{
  // Enable all timers with a callbackStart assigned (used)

  for (uint8_t channelNum = 0; channelNum < MAX_NUMBER_CHANNELS; channelNum++) 
  {
    if (PWM[channelNum].period != 0)
    {
      PWM[channelNum].enabled = true;
    }
  }
}

///////////////////////////////////////////////////

void AVR_Slow_PWM_ISR::disableAll() 
{
  // Disable all timers with a callbackStart assigned (used)
  for (uint8_t channelNum = 0; channelNum < MAX_NUMBER_CHANNELS; channelNum++) 
  {
    if (PWM[channelNum].period != 0)
    {
      PWM[channelNum].enabled = false;
    }
  }
}

///////////////////////////////////////////////////

void AVR_Slow_PWM_ISR::toggle(unsigned channelNum) 
{
  if (channelNum >= MAX_NUMBER_CHANNELS) 
  {
    return;
  }

  PWM[channelNum].enabled = !PWM[channelNum].enabled;
}

///////////////////////////////////////////////////

unsigned AVR_Slow_PWM_ISR::getnumChannels() 
{
  return numChannels;
}

#endif    // AVR_SLOW_PWM_ISR_HPP

