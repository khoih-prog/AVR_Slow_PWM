# AVR_Slow_PWM Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/AVR_Slow_PWM.svg?)](https://www.ardu-badge.com/AVR_Slow_PWM)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/AVR_Slow_PWM.svg)](https://github.com/khoih-prog/AVR_Slow_PWM/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/AVR_Slow_PWM/blob/main/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/AVR_Slow_PWM.svg)](http://github.com/khoih-prog/AVR_Slow_PWM/issues)

<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>

---
---

## Table of Contents

* [Important Change from v1.2.0](#Important-Change-from-v120)
* [Why do we need this AVR_Slow_PWM library](#why-do-we-need-this-AVR_Slow_PWM-library)
  * [Features](#features)
  * [Why using ISR-based PWM is better](#why-using-isr-based-pwm-is-better)
  * [Currently supported Boards](#currently-supported-boards)
  * [Important Notes about ISR](#important-notes-about-isr)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)
* [More useful Information](#more-useful-information)
  * [1. Timer0](#1-timer0)
  * [2. Timer1](#2-timer1)
  * [3. Timer2](#3-timer2)
  * [4. Timer3, Timer4, Timer5](#4-timer3-timer4-timer5)
  * [5. Important Notes](#5-important-notes)
* [Usage](#usage)
  * [1. Init Hardware Timer](#1-init-hardware-timer)
  * [2. Set PWM Frequency, dutycycle, attach irqCallbackStartFunc and irqCallbackStopFunc functions](#2-Set-PWM-Frequency-dutycycle-attach-irqCallbackStartFunc-and-irqCallbackStopFunc-functions)
* [Examples](#examples)
  * [ 1. ISR_8_PWMs_Array](examples/ISR_8_PWMs_Array)
  * [ 2. ISR_8_PWMs_Array_Complex](examples/ISR_8_PWMs_Array_Complex)
  * [ 3. ISR_8_PWMs_Array_Simple](examples/ISR_8_PWMs_Array_Simple)
  * [ 4. ISR_Changing_PWM](examples/ISR_Changing_PWM)
  * [ 5. ISR_Modify_PWM](examples/ISR_Modify_PWM)
  * [ 6. multiFileProject](examples/multiFileProject) **New**
* [Example ISR_8_PWMs_Array_Complex](#Example-ISR_8_PWMs_Array_Complex)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. ISR_8_PWMs_Array_Complex on Arduino AVR Leonardo ATMega32U4](#1-ISR_8_PWMs_Array_Complex-on-Arduino-AVR-Leonardo-ATMega32U4)
  * [2. ISR_8_PWMs_Array on Arduino AVR Mega2560/ADK](#2-isr_8_pwms_array-on-avr-mega2560adk)
  * [3. ISR_8_PWMs_Array_Simple on Arduino AVR Nano](#3-ISR_8_PWMs_Array_Simple-on-Arduino-AVR-Nano)
  * [4. ISR_Modify_PWM on Arduino AVR Mega2560/ADK](#4-ISR_Modify_PWM-on-avr-mega2560adk)
  * [5. ISR_Changing_PWM on Arduino AVR Mega2560/ADK](#5-ISR_Changing_PWM-on-avr-mega2560adk)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---

### Important Change from v1.2.0

Please have a look at [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)

---
---

### Why do we need this [AVR_Slow_PWM library](https://github.com/khoih-prog/AVR_Slow_PWM)

### Features

This library enables you to use ISR-based PWM channels on AVR-based boards, such as Mega-2560, UNO,Nano, Leonardo, etc., using AVR core to create and output PWM any GPIO pin. Because this library doesn't use the powerful purely hardware-controlled PWM with many limitations, the maximum PWM frequency is currently limited at **500Hz**, which is still suitable for many real-life applications. Now you can also modify PWM settings on-the-fly.

---

This library enables you to use Interrupt from Hardware Timers on AVR-based boards to create and output PWM to pins. It now supports 16 ISR-based synchronized PWM channels, while consuming only 1 Hardware Timer. PWM interval can be very long (uint64_t microsecs / millisecs). The most important feature is they're ISR-based PWM channels. Therefore, their executions are not blocked by bad-behaving functions or tasks. This important feature is absolutely necessary for mission-critical tasks. These hardware PWM channels, using interrupt, still work even if other functions are blocking. Moreover, they are much more precise (certainly depending on clock frequency accuracy) than other software PWM using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

As **Hardware Timers are rare, and very precious assets** of any board, this library now enables you to use up to **16 ISR-based synchronized PWM channels, while consuming only 1 Hardware Timer**. Timers' interval is very long (**ulong millisecs**).

Now with these new **16 ISR-based PWM-channels**, the maximum interval is **practically unlimited** (limited only by unsigned long milliseconds) while **the accuracy is nearly perfect** compared to software PWM channels. 

The most important feature is they're ISR-based PWM channels. Therefore, their executions are **not blocked by bad-behaving functions / tasks**. This important feature is absolutely necessary for mission-critical tasks. 

The [**ISR_8_PWMs_Array_Complex**](examples/ISR_8_PWMs_Array_Complex) example will demonstrate the nearly perfect accuracy, compared to software PWM, by printing the actual period / duty-cycle in `microsecs` of each of PWM-channels.

Being ISR-based PWM, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet or Blynk services. You can also have many `(up to 16)` PWM channels to use.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

You'll see `software-based` SimpleTimer is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

---

#### Why using ISR-based PWM is better

Imagine you have a system with a **mission-critical** function, measuring water level and control the sump pump or doing something much more important. You normally use a software timer to poll, or even place the function in loop(). But what if another function is **blocking** the loop() or setup().

So your function **might not be executed, and the result would be disastrous.**

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a Hardware Timer with **Interrupt** to call your function.

These hardware PWM channels, using interrupt, still work even if other functions are blocking. Moreover, they are much more **precise** (certainly depending on clock frequency accuracy) than other software PWM channels using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software PWM channels, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

The catch is **your function is now part of an ISR (Interrupt Service Routine), and must be lean / mean, and follow certain rules.** More to read on:

[**HOWTO Attach Interrupt**](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

---

### Currently supported Boards

1. **AVR-based boards** using ATMEGA_328P, ATMEGA_2560, ATMEGA_1280, ATMEGA_640, ATMEGA_16U4, ATMEGA_32U4, etc. boards such as **Mega-2560, UNO, Nano, Leonardo**, etc., using **Arduino, Adafruit or Sparkfun AVR core**

---

#### Important Notes about ISR

1. Inside the attached function, **delay() won’t work and the value returned by millis() will not increment.** Serial data received while in the function may be lost. You should declare as **volatile any variables that you modify within the attached function.**

2. Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.

---
---

## Prerequisites

 1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
 2. [`Arduino AVR core 1.8.5+`](https://github.com/arduino/ArduinoCore-avr) for Arduino AVR boards. Use Arduino Board Manager to install. [![Latest release](https://img.shields.io/github/release/arduino/ArduinoCore-avr.svg)](https://github.com/arduino/ArduinoCore-avr/releases/latest/)
 3. [`Adafruit AVR core 1.4.14+`](https://github.com/adafruit/Adafruit_Arduino_Boards) for Adafruit AVR boards. Use Arduino Board Manager to install. 
 4. [`Sparkfun AVR core 1.1.13+`](https://github.com/sparkfun/Arduino_Boards) for Sparkfun AVR boards. Use Arduino Board Manager to install. 
 
 5. To use with certain example
   - [`SimpleTimer library`](https://github.com/jfturcot/SimpleTimer) to use with some examples.
   
   
---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for [**AVR_Slow_PWM**](https://github.com/khoih-prog/AVR_Slow_PWM), then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/AVR_Slow_PWM.svg?)](https://www.ardu-badge.com/AVR_Slow_PWM) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**AVR_Slow_PWM**](https://github.com/khoih-prog/AVR_Slow_PWM) page.
2. Download the latest release `AVR_Slow_PWM-master.zip`.
3. Extract the zip file to `AVR_Slow_PWM-master` directory 
4. Copy whole `AVR_Slow_PWM-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**AVR_Slow_PWM** library](https://registry.platformio.org/libraries/khoih-prog/AVR_Slow_PWM) by using [Library Manager](https://registry.platformio.org/libraries/khoih-prog/AVR_Slow_PWM/installation). Search for **AVR_Slow_PWM** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)


---
---

### HOWTO Fix `Multiple Definitions` Linker Error

The current library implementation, using `xyz-Impl.h` instead of standard `xyz.cpp`, possibly creates certain `Multiple Definitions` Linker error in certain use cases.

You can include this `.hpp` file

```
// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "AVR_Slow_PWM.hpp"     //https://github.com/khoih-prog/AVR_Slow_PWM
```

in many files. But be sure to use the following `.h` file **in just 1 `.h`, `.cpp` or `.ino` file**, which must **not be included in any other file**, to avoid `Multiple Definitions` Linker Error

```
// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "AVR_Slow_PWM.h"           //https://github.com/khoih-prog/AVR_Slow_PWM
```

Check the new [**multiFileProject** example](examples/multiFileProject) for a `HOWTO` demo.

Have a look at the discussion in [Different behaviour using the src_cpp or src_h lib #80](https://github.com/khoih-prog/ESPAsync_WiFiManager/discussions/80)


---
---

## More useful Information

From [Arduino 101: Timers and Interrupts](https://www.robotshop.com/community/forum/t/arduino-101-timers-and-interrupts/13072)

### 1. Timer0:

Timer0 is a 8-bit timer.

In the Arduino world, **Timer0 is been used for the timer functions**, like delay(), millis() and micros(). If you change Timer0 registers, this may influence the Arduino timer function. So you should know what you are doing.

### 2. Timer1:

**Timer1** is a 16-bit timer. In the Arduino world, the Servo library uses Timer1 on Arduino Uno (Timer5 on Arduino Mega).

### 3. Timer2:

**Timer2** is a 8-bit timer like Timer0. This Timer2 is not available for **ATMEGA_16U4, ATMEGA_32U4 boards**, such as Leonardo, YUN, ESPLORA, etc.
In the Arduino world, the **tone() function uses Timer2**.

### 4. Timer3, Timer4, Timer5:

**Timer4** is only available on Arduino **ATMEGA_2560, ATMEGA_1280, ATMEGA_640, ATMEGA_16U4, ATMEGA_32U4 boards**. This Timer4 is 16-bit timer on **ATMEGA_2560, ATMEGA_1280, ATMEGA_640 boards** and 10-bit (but used as 8-bit in this library) Timer on **ATMEGA_16U4, ATMEGA_32U4 boards**

**Timer3 and Timer5** are only available on Arduino Mega boards. These 2 timers are all 16-bit timers.


### 5. Important Notes

Before using any Timer, you have to make sure the **Timer has not been used by any other purpose.**


#### Timer1,3,4 are supported for ATMEGA_16U4, ATMEGA_32U4 boards, such as Leonardo, YUN, ESPLORA, etc.

#### Only Timer1 and Timer2 are supported for Nano, UNO, etc. boards possessing 3 timers.

#### Timer1-Timer5 are available for Arduino Mega boards.

---
---

## Usage


Before using any Timer, you have to make sure the Timer has not been used by any other purpose.


#### 1. Init Hardware Timer

```
// Select the timers you're using, here ITimer1
#define USE_TIMER_1     true
#define USE_TIMER_2     false
#define USE_TIMER_3     false
#define USE_TIMER_4     false
#define USE_TIMER_5     false

// Init AVR_Slow_PWM, each can service 16 different ISR-based PWM channels
AVR_Slow_PWM ISR_PWM;
```

#### 2. Set PWM Frequency, dutycycle, attach irqCallbackStartFunc and irqCallbackStopFunc functions

```
void irqCallbackStartFunc()
{

}

void irqCallbackStopFunc()
{

}

void setup()
{
  ....
  
  // You can use this with PWM_Freq in Hz
  ISR_PWM.setPWM(PWM_Pin, PWM_Freq, PWM_DutyCycle, irqCallbackStartFunc, irqCallbackStopFunc);
                   
  ....                 
}  
```

---
---

### Examples: 

 1. [ISR_8_PWMs_Array](examples/ISR_8_PWMs_Array)
 2. [ISR_8_PWMs_Array_Complex](examples/ISR_8_PWMs_Array_Complex)
 3. [ISR_8_PWMs_Array_Simple](examples/ISR_8_PWMs_Array_Simple)
 4. [ISR_Changing_PWM](examples/ISR_Changing_PWM)
 5. [ISR_Modify_PWM](examples/ISR_Modify_PWM)
 6. [**multiFileProject**](examples/multiFileProject) **New** 

 
---
---

### Example [ISR_8_PWMs_Array_Complex](examples/ISR_8_PWMs_Array_Complex)

https://github.com/khoih-prog/AVR_Slow_PWM/blob/e52cd46014064efa2b30c895ce14c7cd5c850e27/examples/ISR_8_PWMs_Array_Complex/ISR_8_PWMs_Array_Complex.ino#L22-L508

---
---

### Debug Terminal Output Samples

### 1. ISR_8_PWMs_Array_Complex on Arduino AVR Leonardo ATMega32U4

The following is the sample terminal output when running example [ISR_8_PWMs_Array_Complex](examples/ISR_8_PWMs_Array_Complex) to demonstrate how to use multiple PWM channels with complex callback functions, the accuracy of ISR Hardware PWM-channels, **especially when system is very busy**.  The ISR PWM-channels is **running exactly according to corresponding programmed periods and duty-cycles**


```
Starting ISR_8_PWMs_Array_Complex on Arduino AVR ATMega32U4
AVR_Slow_PWM v1.2.3
CPU Frequency = 16 MHz
[PWM] T3
[PWM] Freq * 1000 = 10000000.00
[PWM] F_CPU = 16000000 , preScalerDiv = 1
[PWM] OCR = 1599 , preScalerIndex = 1
[PWM] OK in loop => _OCR = 1599
[PWM] _preScalerIndex = 1 , preScalerDiv = 1
Starting  ITimer3 OK, micros() = 3520328
Channel : 0	Period : 1000000		OnTime : 50000	Start_Time : 3520968
Channel : 1	Period : 500000		OnTime : 50000	Start_Time : 3520968
Channel : 2	Period : 333333		OnTime : 66666	Start_Time : 3520968
Channel : 3	Period : 250000		OnTime : 62500	Start_Time : 3520968
Channel : 4	Period : 200000		OnTime : 60000	Start_Time : 3520968
Channel : 5	Period : 166666		OnTime : 58333	Start_Time : 3520968
Channel : 6	Period : 142857		OnTime : 57142	Start_Time : 3520968
Channel : 7	Period : 125000		OnTime : 56250	Start_Time : 3520968
SimpleTimer (us): 2000, us : 13547548, Dus : 10026624
PWM Channel : 0, prog Period (ms): 1000.00, actual (uS) : 1000000, prog DutyCycle : 5, actual : 5.00
PWM Channel : 1, prog Period (ms): 500.00, actual (uS) : 500016, prog DutyCycle : 10, actual : 10.00
PWM Channel : 2, prog Period (ms): 333.33, actual (uS) : 333396, prog DutyCycle : 20, actual : 19.98
PWM Channel : 3, prog Period (ms): 250.00, actual (uS) : 250028, prog DutyCycle : 25, actual : 24.96
PWM Channel : 4, prog Period (ms): 200.00, actual (uS) : 200192, prog DutyCycle : 30, actual : 29.87
PWM Channel : 5, prog Period (ms): 166.67, actual (uS) : 166792, prog DutyCycle : 35, actual : 34.89
PWM Channel : 6, prog Period (ms): 142.86, actual (uS) : 142992, prog DutyCycle : 40, actual : 39.87
PWM Channel : 7, prog Period (ms): 125.00, actual (uS) : 125192, prog DutyCycle : 45, actual : 44.89
SimpleTimer (us): 2000, us : 23596540, Dus : 10048992
PWM Channel : 0, prog Period (ms): 1000.00, actual (uS) : 1000000, prog DutyCycle : 5, actual : 5.00
PWM Channel : 1, prog Period (ms): 500.00, actual (uS) : 500012, prog DutyCycle : 10, actual : 10.00
PWM Channel : 2, prog Period (ms): 333.33, actual (uS) : 333396, prog DutyCycle : 20, actual : 19.98
PWM Channel : 3, prog Period (ms): 250.00, actual (uS) : 250028, prog DutyCycle : 25, actual : 24.95
PWM Channel : 4, prog Period (ms): 200.00, actual (uS) : 200192, prog DutyCycle : 30, actual : 29.88
PWM Channel : 5, prog Period (ms): 166.67, actual (uS) : 166796, prog DutyCycle : 35, actual : 34.90
PWM Channel : 6, prog Period (ms): 142.86, actual (uS) : 142992, prog DutyCycle : 40, actual : 39.87
PWM Channel : 7, prog Period (ms): 125.00, actual (uS) : 125192, prog DutyCycle : 45, actual : 44.89
```

---

### 2. ISR_8_PWMs_Array on AVR Mega2560/ADK

The following is the sample terminal output when running example [**ISR_8_PWMs_Array**](examples/ISR_8_PWMs_Array) on **AVR Mega2560/ADK** to demonstrate how to use multiple PWM channels with simple callback functions.

```
Starting ISR_8_PWMs_Array on Arduino AVR Mega2560/ADK
AVR_Slow_PWM v1.2.3
CPU Frequency = 16 MHz
[PWM] T3
[PWM] Freq * 1000 = 10000000.00
[PWM] F_CPU = 16000000 , preScalerDiv = 1
[PWM] OCR = 1599 , preScalerIndex = 1
[PWM] OK in loop => _OCR = 1599
[PWM] _preScalerIndex = 1 , preScalerDiv = 1
Starting  ITimer3 OK, micros() = 2023732
Channel : 0	    Period : 1000000		OnTime : 50000	Start_Time : 2031420
Channel : 1	    Period : 500000		OnTime : 50000	Start_Time : 2042728
Channel : 2	    Period : 333333		OnTime : 66666	Start_Time : 2054724
Channel : 3	    Period : 250000		OnTime : 62500	Start_Time : 2067932
Channel : 4	    Period : 200000		OnTime : 60000	Start_Time : 2084232
Channel : 5	    Period : 166666		OnTime : 58333	Start_Time : 2115432
Channel : 6	    Period : 142857		OnTime : 57142	Start_Time : 3075536
Channel : 7	    Period : 125000		OnTime : 56250	Start_Time : 4221280
```

---

### 3. ISR_8_PWMs_Array_Simple on Arduino AVR Nano

The following is the sample terminal output when running example [**ISR_8_PWMs_Array_Simple**](examples/ISR_8_PWMs_Array_Simple) on **Arduino AVR UNO** to demonstrate how to use multiple PWM channels.

```
Starting ISR_8_PWMs_Array_Complex on Arduino AVR UNO, Nano, etc.
AVR_Slow_PWM v1.2.3
CPU Frequency = 16 MHz
[PWM] T1
[PWM] Freq * 1000 = 10000000.00
[PWM] F_CPU = 16000000 , preScalerDiv = 1
[PWM] OCR = 1599 , preScalerIndex = 1
[PWM] OK in loop => _OCR = 1599
[PWM] _preScalerIndex = 1 , preScalerDiv = 1
[PWM] TCCR1B = 9
Starting  ITimer1 OK, micros() = 2025876
Channel : 0	Period : 1000000		OnTime : 50000	Start_Time : 2026740
Channel : 1	Period : 500000		OnTime : 50000	Start_Time : 2026740
Channel : 2	Period : 333333		OnTime : 66666	Start_Time : 2026740
Channel : 3	Period : 250000		OnTime : 62500	Start_Time : 2026740
Channel : 4	Period : 200000		OnTime : 60000	Start_Time : 2026740
Channel : 5	Period : 166666		OnTime : 58333	Start_Time : 2026740
Channel : 6	Period : 142857		OnTime : 57142	Start_Time : 2026740
Channel : 7	Period : 125000		OnTime : 56250	Start_Time : 2026740
SimpleTimer (us): 2000, us : 12072128, Dus : 10045452
PWM Channel : 0, prog Period (ms): 1000.00, actual (uS) : 1000000, prog DutyCycle : 5, actual : 5.00
PWM Channel : 1, prog Period (ms): 500.00, actual (uS) : 500012, prog DutyCycle : 10, actual : 10.00
PWM Channel : 2, prog Period (ms): 333.33, actual (uS) : 333396, prog DutyCycle : 20, actual : 19.98
PWM Channel : 3, prog Period (ms): 250.00, actual (uS) : 250024, prog DutyCycle : 25, actual : 24.95
PWM Channel : 4, prog Period (ms): 200.00, actual (uS) : 200208, prog DutyCycle : 30, actual : 29.96
PWM Channel : 5, prog Period (ms): 166.67, actual (uS) : 166792, prog DutyCycle : 35, actual : 34.90
PWM Channel : 6, prog Period (ms): 142.86, actual (uS) : 142992, prog DutyCycle : 40, actual : 39.87
PWM Channel : 7, prog Period (ms): 125.00, actual (uS) : 125192, prog DutyCycle : 45, actual : 44.89
SimpleTimer (us): 2000, us : 22147164, Dus : 10075036
PWM Channel : 0, prog Period (ms): 1000.00, actual (uS) : 1000000, prog DutyCycle : 5, actual : 5.00
PWM Channel : 1, prog Period (ms): 500.00, actual (uS) : 500012, prog DutyCycle : 10, actual : 10.00
PWM Channel : 2, prog Period (ms): 333.33, actual (uS) : 333396, prog DutyCycle : 20, actual : 19.98
PWM Channel : 3, prog Period (ms): 250.00, actual (uS) : 250196, prog DutyCycle : 25, actual : 24.94
PWM Channel : 4, prog Period (ms): 200.00, actual (uS) : 199996, prog DutyCycle : 30, actual : 30.00
PWM Channel : 5, prog Period (ms): 166.67, actual (uS) : 166792, prog DutyCycle : 35, actual : 34.90
PWM Channel : 6, prog Period (ms): 142.86, actual (uS) : 143012, prog DutyCycle : 40, actual : 39.87
PWM Channel : 7, prog Period (ms): 125.00, actual (uS) : 125012, prog DutyCycle : 45, actual : 44.95
```

---

### 4. ISR_Modify_PWM on AVR Mega2560/ADK

The following is the sample terminal output when running example [ISR_Modify_PWM](examples/ISR_Modify_PWM) on **AVR Mega2560/ADK** to demonstrate how to modify PWM settings on-the-fly without deleting the PWM channel

```
Starting ISR_Modify_PWM on Arduino AVR Mega2560/ADK
AVR_Slow_PWM v1.2.3
CPU Frequency = 16 MHz
[PWM] T3
[PWM] Freq * 1000 = 10000000.00
[PWM] F_CPU = 16000000 , preScalerDiv = 1
[PWM] OCR = 1599 , preScalerIndex = 1
[PWM] OK in loop => _OCR = 1599
[PWM] _preScalerIndex = 1 , preScalerDiv = 1
Starting  ITimer3 OK, micros() = 2023764
Using PWM Freq = 200.00, PWM DutyCycle = 1.00
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 2038456
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 12052172
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 22052172
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 32052172
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 42057172
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 52057180
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 62057372
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 72057372
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 82057376
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 92057572
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 102062572
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 112062572
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 122062572
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 132062572
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 142067572
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 152062572
```

---

### 5. ISR_Changing_PWM on AVR Mega2560/ADK

The following is the sample terminal output when running example [ISR_Changing_PWM](examples/ISR_Changing_PWM) on **AVR Mega2560/ADK** to demonstrate how to modify PWM settings on-the-fly by deleting the PWM channel and reinit the PWM channel

```
Starting ISR_Changing_PWM on Arduino AVR Mega2560/ADK
AVR_Slow_PWM v1.2.3
CPU Frequency = 16 MHz
[PWM] T3
[PWM] Freq * 1000 = 10000000.00
[PWM] F_CPU = 16000000 , preScalerDiv = 1
[PWM] OCR = 1599 , preScalerIndex = 1
[PWM] OK in loop => _OCR = 1599
[PWM] _preScalerIndex = 1 , preScalerDiv = 1
Starting  ITimer3 OK, micros() = 2023844
Using PWM Freq = 1.00, PWM DutyCycle = 50.00
Using PWM Freq = 2.00, PWM DutyCycle = 90.00
Using PWM Freq = 1.00, PWM DutyCycle = 50.00
Using PWM Freq = 2.00, PWM DutyCycle = 90.00
Using PWM Freq = 1.00, PWM DutyCycle = 50.00
Using PWM Freq = 2.00, PWM DutyCycle = 90.00
```

---
---

### Debug

Debug is enabled by default on Serial.

You can also change the debugging level `_PWM_LOGLEVEL_` from 0 to 4

```cpp
// Don't define _PWM_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define _PWM_LOGLEVEL_     0
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the core for Arduino boards.

Sometimes, the library will only work if you update the board core to the latest version because I am using newly added functions.


---
---

### Issues

Submit issues to: [AVR_Slow_PWM issues](https://github.com/khoih-prog/AVR_Slow_PWM/issues)

---

## TO DO

1. Search for bug and improvement.
2. Similar features for remaining Arduino boards

---

## DONE

1. Basic hardware multi-channel PWM for **AVR boards, such as Mega-2560, UNO,Nano, Leonardo, etc.** using AVR core
2. Add Table of Contents
3. Add functions to modify PWM settings on-the-fly
4. Fix `multiple-definitions` linker error. Drop `src_cpp` and `src_h` directories
5. Add example [multiFileProject](examples/multiFileProject) to demo for multiple-file project
6. Improve accuracy by using `float`, instead of `uint32_t` for `dutycycle`
7. Optimize library code by using `reference-passing` instead of `value-passing`
8. DutyCycle to be optionally updated at the end current PWM period instead of immediately.
9. Display informational warning only when `_PWM_LOGLEVEL_` > 3

---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library.


---

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [MIT](https://github.com/khoih-prog/AVR_Slow_PWM/blob/main/LICENSE)

---

## Copyright

Copyright 2021- Khoi Hoang


