/****************************************************************************************************************************
  AVR_Slow_PWM_ISR.h
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

#ifndef AVR_SLOW_PWM_ISR_H
#define AVR_SLOW_PWM_ISR_H

#if defined(BOARD_NAME)
  #undef BOARD_NAME
#endif

#if ( defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || \
      defined(__AVR_ATmega640__) || defined(__AVR_ATmega641__))
  #if defined(TIMER_INTERRUPT_USING_ATMEGA2560)
    #undef TIMER_INTERRUPT_USING_ATMEGA2560
  #endif
  #define TIMER_INTERRUPT_USING_ATMEGA2560      true   
  #define BOARD_NAME    F("Arduino AVR Mega2560/ADK")
  #warning Using Arduino AVR Mega, Mega640(P), Mega2560/ADK. Timer1-5 available
  
#elif ( defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)  || \
        defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_MINI) ||    defined(ARDUINO_AVR_ETHERNET) || \
        defined(ARDUINO_AVR_FIO) || defined(ARDUINO_AVR_BT)   || defined(ARDUINO_AVR_LILYPAD) || defined(ARDUINO_AVR_PRO)      || \
        defined(ARDUINO_AVR_NG) || defined(ARDUINO_AVR_UNO_WIFI_DEV_ED) || defined(ARDUINO_AVR_DUEMILANOVE) )      
  #define BOARD_NAME    "Arduino AVR UNO, Nano, etc." 
  #warning Using Aduino AVR ATMega644(P), ATMega328(P) such as UNO, Nano. Only Timer1,2 available

#elif ( defined(ARDUINO_AVR_FEATHER328P) || defined(ARDUINO_AVR_METRO) || defined(ARDUINO_AVR_PROTRINKET5) || defined(ARDUINO_AVR_PROTRINKET3) || \
      defined(ARDUINO_AVR_PROTRINKET5FTDI) || defined(ARDUINO_AVR_PROTRINKET3FTDI) )
  #define BOARD_NAME    F("Adafruit AVR ATMega328(P)")
  #warning Using Adafruit ATMega328(P), such as AVR_FEATHER328P or AVR_METRO. Only Timer1,2 available
        
#elif ( defined(ARDUINO_AVR_LEONARDO) || defined(ARDUINO_AVR_LEONARDO_ETH) || defined(ARDUINO_AVR_YUN) || defined(ARDUINO_AVR_MICRO) || \
        defined(ARDUINO_AVR_ESPLORA)  || defined(ARDUINO_AVR_LILYPAD_USB)  || defined(ARDUINO_AVR_ROBOT_CONTROL) || defined(ARDUINO_AVR_ROBOT_MOTOR) || \
        defined(ARDUINO_AVR_CIRCUITPLAY)  || defined(ARDUINO_AVR_YUNMINI) || defined(ARDUINO_AVR_INDUSTRIAL101) || defined(ARDUINO_AVR_LININO_ONE) )
  #if defined(TIMER_INTERRUPT_USING_ATMEGA_32U4)
    #undef TIMER_INTERRUPT_USING_ATMEGA_32U4
  #endif
  #define TIMER_INTERRUPT_USING_ATMEGA_32U4      true
  #define BOARD_NAME    F("Arduino AVR ATMega32U4")
  #warning Using Arduino ATMega32U4, such as Leonardo or Leonardo ETH. Only Timer1,3,4 available
  
#elif ( defined(ARDUINO_AVR_FLORA8 ) || defined(ARDUINO_AVR_FEATHER32U4) || defined(ARDUINO_AVR_CIRCUITPLAY) || defined(ARDUINO_AVR_ITSYBITSY32U4_5V) || \
        defined(ARDUINO_AVR_ITSYBITSY32U4_3V)  || defined(ARDUINO_AVR_BLUEFRUITMICRO) || defined(ARDUINO_AVR_ADAFRUIT32U4) )
  #if defined(TIMER_INTERRUPT_USING_ATMEGA_32U4)
    #undef TIMER_INTERRUPT_USING_ATMEGA_32U4
  #endif
  #define TIMER_INTERRUPT_USING_ATMEGA_32U4      true
  #define BOARD_NAME    F("Adafruit AVR ATMega32U4")
  #warning Using Adafruit ATMega32U4, such as Feather_32u4, AVR_CIRCUITPLAY, etc.. Only Timer1,3,4 available

#elif ( defined(__AVR_ATmega32U4__) || defined(ARDUINO_AVR_MAKEYMAKEY ) || defined(ARDUINO_AVR_PROMICRO) || defined(ARDUINO_AVR_FIOV3) || \
        defined(ARDUINO_AVR_QDUINOMINI) || defined(ARDUINO_AVR_LILYPAD_ARDUINO_USB_PLUS_BOARD ) )
  #if defined(TIMER_INTERRUPT_USING_ATMEGA_32U4)
    #undef TIMER_INTERRUPT_USING_ATMEGA_32U4
  #endif
  #define TIMER_INTERRUPT_USING_ATMEGA_32U4      true
  #define BOARD_NAME    F("Generic or Sparkfun AVR ATMega32U4")
  #warning Using Generic ATMega32U4, such as Sparkfun AVR_MAKEYMAKEY, AVR_PROMICRO, etc. Only Timer1,3,4 available

#elif ( defined(__AVR_ATmega328P__) || defined(ARDUINO_AVR_DIGITAL_SANDBOX ) || defined(ARDUINO_REDBOT) || defined(ARDUINO_AVR_SERIAL_7_SEGMENT) )
  #define BOARD_NAME    F("Generic or Sparkfun AVR ATMega328P")
  #warning Using Generic ATMega328P, such as Sparkfun AVR_DIGITAL_SANDBOX, REDBOT, etc.

#elif ( defined(__AVR_ATmega128RFA1__) || defined(ARDUINO_ATMEGA128RFA1_DEV_BOARD) )
  #define BOARD_NAME    F("Generic or Sparkfun AVR ATMega128RFA1")
  #warning Using Generic ATMega128RFA1, such as Sparkfun ATMEGA128RFA1_DEV_BOARD, etc.
  
#elif ( defined(ARDUINO_AVR_GEMMA) || defined(ARDUINO_AVR_TRINKET3) || defined(ARDUINO_AVR_TRINKET5) )
  #error These AVR boards are not supported! Please check your Tools->Board setting.
     
#else
  #error This is designed only for Arduino or Adafruit AVR board! Please check your Tools->Board setting.
#endif

#ifndef AVR_SLOW_PWM_VERSION
  #define AVR_SLOW_PWM_VERSION       F("AVR_Slow_PWM v1.1.0")
#endif

#ifndef _PWM_LOGLEVEL_
  #define _PWM_LOGLEVEL_        1
#endif

#include "PWM_Generic_Debug.h"

#include <stddef.h>

#include <inttypes.h>

#if defined(ARDUINO)
  #if ARDUINO >= 100
    #include <Arduino.h>
  #else
    #include <WProgram.h>
  #endif
#endif

#define AVR_Slow_PWM_ISR  AVR_Slow_PWM

typedef void (*timer_callback)();
typedef void (*timer_callback_p)(void *);

#if !defined(USING_MICROS_RESOLUTION)
  #warning Not USING_MICROS_RESOLUTION, using millis resolution
  #define USING_MICROS_RESOLUTION       false
#endif

#define INVALID_AVR_PIN         255

//////////////////////////////////////////////////////////////////

class AVR_Slow_PWM_ISR 
{

  public:
    // maximum number of PWM channels
#define MAX_NUMBER_CHANNELS        16

    // constructor
    AVR_Slow_PWM_ISR();

    void init();

    // this function must be called inside loop()
    void run();
    
    //////////////////////////////////////////////////////////////////
    // PWM
    // Return the channelNum if OK, -1 if error
    int setPWM(uint32_t pin, double frequency, uint32_t dutycycle, timer_callback StartCallback = nullptr, 
                timer_callback StopCallback = nullptr)
    {
      uint32_t period = 0;
      
     if ( ( frequency != 0 ) && ( frequency <= 1000 ) )
      {
#if USING_MICROS_RESOLUTION
      // period in us
      period = 1000000.0f / frequency;
#else    
      // period in ms
      period = 1000.0f / frequency;
#endif
      }
      else
      {       
        PWM_LOGERROR("Error: Invalid frequency, max is 1000Hz");
        
        return -1;
      }
      
      return setupPWMChannel(pin, period, dutycycle, (void *) StartCallback, (void *) StopCallback);   
    }

    // period in us
    // Return the channelNum if OK, -1 if error
    int setPWM_Period(uint32_t pin, uint32_t period, uint32_t dutycycle, timer_callback StartCallback = nullptr,
                       timer_callback StopCallback = nullptr)  
    {     
      return setupPWMChannel(pin, period, dutycycle, (void *) StartCallback, (void *) StopCallback);      
    } 
    
    //////////////////////////////////////////////////////////////////
    
    // low level function to modify a PWM channel
    // returns the true on success or false on failure
    bool modifyPWMChannel(unsigned channelNum, uint32_t pin, double frequency, uint32_t dutycycle)
    {
      uint32_t period = 0;
      
      if ( ( frequency > 0 ) && ( frequency <= 1000 ) )
      {
#if USING_MICROS_RESOLUTION
      // period in us
      period = 1000000.0f / frequency;
#else    
      // period in ms
      period = 1000.0f / frequency;
#endif
      }
      else
      {       
        PWM_LOGERROR("Error: Invalid frequency, max is 1000Hz");
        return false;
      }
      
      return modifyPWMChannel_Period(channelNum, pin, period, dutycycle);
    }
    
    //////////////////////////////////////////////////////////////////
    
    //period in us
    bool modifyPWMChannel_Period(unsigned channelNum, uint32_t pin, uint32_t period, uint32_t dutycycle);
    
    //////////////////////////////////////////////////////////////////

    // destroy the specified PWM channel
    void deleteChannel(unsigned channelNum);

    // restart the specified PWM channel
    void restartChannel(unsigned channelNum);

    // returns true if the specified PWM channel is enabled
    bool isEnabled(unsigned channelNum);

    // enables the specified PWM channel
    void enable(unsigned channelNum);

    // disables the specified PWM channel
    void disable(unsigned channelNum);

    // enables all PWM channels
    void enableAll();

    // disables all PWM channels
    void disableAll();

    // enables the specified PWM channel if it's currently disabled, and vice-versa
    void toggle(unsigned channelNum);

    // returns the number of used PWM channels
    unsigned getnumChannels();

    // returns the number of available PWM channels
    unsigned getNumAvailablePWMChannels() 
    {
      return MAX_NUMBER_CHANNELS - numChannels;
    };

  private:

    // low level function to initialize and enable a new PWM channel
    // returns the PWM channel number (channelNum) on success or
    // -1 on failure (f == NULL) or no free PWM channels 
    int setupPWMChannel(uint32_t pin, uint32_t period, uint32_t dutycycle, void* cbStartFunc = nullptr, void* cbStopFunc = nullptr);

    // find the first available slot
    int findFirstFreeSlot();

    typedef struct 
    {
      ///////////////////////////////////
      
      
      ///////////////////////////////////
      
      uint32_t      prevTime;           // value returned by the micros() or millis() function in the previous run() call
      uint32_t      period;             // period value, in us / ms
      uint32_t      onTime;             // onTime value, ( period * dutyCycle / 100 ) us  / ms
      
      void*         callbackStart;      // pointer to the callback function when PWM pulse starts (HIGH)
      void*         callbackStop;       // pointer to the callback function when PWM pulse stops (LOW)
      
      ////////////////////////////////////////////////////////////
      
      uint32_t      pin;                // PWM pin
      bool          pinHigh;            // true if PWM pin is HIGH
      ////////////////////////////////////////////////////////////
      
      bool          enabled;            // true if enabled
    } PWM_t;

    volatile PWM_t PWM[MAX_NUMBER_CHANNELS];

    // actual number of PWM channels in use (-1 means uninitialized)
    volatile int numChannels;
};


#endif    // AVR_SLOW_PWM_ISR_H


