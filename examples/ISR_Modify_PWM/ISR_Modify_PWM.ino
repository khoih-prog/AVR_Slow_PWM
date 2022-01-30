/****************************************************************************************************************************
  ISR_Modify_PWM.ino
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
*****************************************************************************************************************************/

#if ( defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)  || \
        defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_MINI) ||    defined(ARDUINO_AVR_ETHERNET) || \
        defined(ARDUINO_AVR_FIO) || defined(ARDUINO_AVR_BT)   || defined(ARDUINO_AVR_LILYPAD) || defined(ARDUINO_AVR_PRO)      || \
        defined(ARDUINO_AVR_NG) || defined(ARDUINO_AVR_UNO_WIFI_DEV_ED) || defined(ARDUINO_AVR_DUEMILANOVE) || defined(ARDUINO_AVR_FEATHER328P) || \
        defined(ARDUINO_AVR_METRO) || defined(ARDUINO_AVR_PROTRINKET5) || defined(ARDUINO_AVR_PROTRINKET3) || defined(ARDUINO_AVR_PROTRINKET5FTDI) || \
        defined(ARDUINO_AVR_PROTRINKET3FTDI) )
  #define USE_TIMER_1     true
  #warning Using Timer1
#else          
  #define USE_TIMER_3     true
  #warning Using Timer3
#endif

// These define's must be placed at the beginning before #include "ESP32_PWM.h"
// _PWM_LOGLEVEL_ from 0 to 4
// Don't define _PWM_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define _PWM_LOGLEVEL_      3

#define USING_MICROS_RESOLUTION       true    //false

// Default is true, uncomment to false
//#define CHANGING_PWM_END_OF_CYCLE     false

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "AVR_Slow_PWM.h"

#define LED_OFF             HIGH
#define LED_ON              LOW

#ifndef LED_BUILTIN
  #define LED_BUILTIN       13
#endif

#ifndef LED_BLUE
  #define LED_BLUE          10
#endif

#ifndef LED_RED
  #define LED_RED           11
#endif

// Don't change these numbers to make higher Timer freq. System can hang
#define HW_TIMER_INTERVAL_MS        0.1f
#define HW_TIMER_INTERVAL_FREQ      10000L

volatile uint32_t startMicros = 0;

// Init AVR_Slow_PWM, each can service 16 different ISR-based PWM channels
AVR_Slow_PWM ISR_PWM;

//////////////////////////////////////////////////////

void TimerHandler()
{ 
  ISR_PWM.run();
}

//////////////////////////////////////////////////////

#define USING_PWM_FREQUENCY     false //true

//////////////////////////////////////////////////////

// You can assign pins here. Be carefull to select good pin to use or crash
uint32_t PWM_Pin    = LED_BUILTIN;

// You can assign any interval for any timer here, in Hz
double PWM_Freq1   = 1.0f;
// You can assign any interval for any timer here, in Hz
double PWM_Freq2   = 2.0f;

// You can assign any interval for any timer here, in microseconds
double PWM_Period1 = 1000000.0 / PWM_Freq1;
// You can assign any interval for any timer here, in microseconds
double PWM_Period2 = 1000000.0 / PWM_Freq2;

// You can assign any duty_cycle for any PWM here, from 0-100
double PWM_DutyCycle1  = 10.0;
// You can assign any duty_cycle for any PWM here, from 0-100
double PWM_DutyCycle2  = 90.0;

// Channel number used to identify associated channel
int channelNum;

////////////////////////////////////////////////

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  delay(2000);

  Serial.print(F("\nStarting ISR_Modify_PWM on ")); Serial.println(BOARD_NAME);
  Serial.println(AVR_SLOW_PWM_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  // Timer0 is used for micros(), millis(), delay(), etc and can't be used
  // Select Timer 1-2 for UNO, 1-5 for MEGA, 1,3,4 for 16u4/32u4
  // Timer 2 is 8-bit timer, only for higher frequency
  // Timer 4 of 16u4 and 32u4 is 8/10-bit timer, only for higher frequency

#if USING_HW_TIMER_INTERVAL_MS

#if USE_TIMER_1

  ITimer1.init();

  // Using ATmega328 used in UNO => 16MHz CPU clock ,

  if (ITimer1.attachInterruptInterval(HW_TIMER_INTERVAL_MS, TimerHandler))
  {
    Serial.print(F("Starting  ITimer1 OK, micros() = ")); Serial.println(micros());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
    
#elif USE_TIMER_3

  ITimer3.init();

  if (ITimer3.attachInterruptInterval(HW_TIMER_INTERVAL_MS, TimerHandler))
  {
    Serial.print(F("Starting  ITimer3 OK, micros() = ")); Serial.println(micros());
  }
  else
    Serial.println(F("Can't set ITimer3. Select another freq. or timer"));

#endif

#else

#if USE_TIMER_1

  ITimer1.init();

  // Using ATmega328 used in UNO => 16MHz CPU clock ,

  if (ITimer1.attachInterrupt(HW_TIMER_INTERVAL_FREQ, TimerHandler))
  {
    Serial.print(F("Starting  ITimer1 OK, micros() = ")); Serial.println(micros());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
    
#elif USE_TIMER_3

  ITimer3.init();

  if (ITimer3.attachInterrupt(HW_TIMER_INTERVAL_FREQ, TimerHandler))
  {
    Serial.print(F("Starting  ITimer3 OK, micros() = ")); Serial.println(micros());
  }
  else
    Serial.println(F("Can't set ITimer3. Select another freq. or timer"));

#endif

#endif

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_PWM
  //void setPWM(uint32_t pin, uint32_t frequency, uint32_t dutycycle
  // , timer_callback_p StartCallback = nullptr, timer_callback_p StopCallback = nullptr)
  Serial.print(F("Using PWM Freq = ")); Serial.print(PWM_Freq1); Serial.print(F(", PWM DutyCycle = ")); Serial.println(PWM_DutyCycle1);

#if USING_PWM_FREQUENCY

  // You can use this with PWM_Freq in Hz
  ISR_PWM.setPWM(PWM_Pin, PWM_Freq1, PWM_DutyCycle1);

#else
#if USING_MICROS_RESOLUTION
  // Or using period in microsecs resolution
  channelNum = ISR_PWM.setPWM_Period(PWM_Pin, PWM_Period1, PWM_DutyCycle1);
#else
  // Or using period in millisecs resolution
  channelNum = ISR_PWM.setPWM_Period(PWM_Pin, PWM_Period1 / 1000.0, PWM_DutyCycle1);
#endif
#endif
}

////////////////////////////////////////////////

void changePWM()
{
  static uint8_t count = 1;

  double PWM_Freq;
  uint32_t PWM_DutyCycle;

  if (count++ % 2)
  {
    PWM_Freq        = PWM_Freq2;
    PWM_DutyCycle   = PWM_DutyCycle2;
  }
  else
  {
    PWM_Freq        = PWM_Freq1;
    PWM_DutyCycle   = PWM_DutyCycle1;
  }

  // You can use this with PWM_Freq in Hz
  if (!ISR_PWM.modifyPWMChannel(channelNum, PWM_Pin, PWM_Freq, PWM_DutyCycle))
  {
    Serial.print(F("modifyPWMChannel error for PWM_Period"));
  }
}

////////////////////////////////////////////////

void changingPWM()
{
  static unsigned long changingPWM_timeout = 0;

  static unsigned long current_millis;

#define CHANGING_PWM_INTERVAL    10000L

  current_millis = millis();

  // changePWM every CHANGING_PWM_INTERVAL (10) seconds.
  if ( (current_millis > changingPWM_timeout) )
  {
    if (changingPWM_timeout > 0)
      changePWM();

    changingPWM_timeout = current_millis + CHANGING_PWM_INTERVAL;
  }
}

////////////////////////////////////////////////

void loop()
{
  changingPWM();
}
