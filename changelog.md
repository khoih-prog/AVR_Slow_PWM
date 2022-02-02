# AVR_Slow_PWM Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/AVR_Slow_PWM.svg?)](https://www.ardu-badge.com/AVR_Slow_PWM)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/AVR_Slow_PWM.svg)](https://github.com/khoih-prog/AVR_Slow_PWM/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/AVR_Slow_PWM/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/AVR_Slow_PWM.svg)](http://github.com/khoih-prog/AVR_Slow_PWM/issues)

---
---

## Table of Contents

* [Changelog](#changelog)
  * [Releases v1.2.2](#releases-v122)
  * [Releases v1.2.1](#releases-v121)
  * [Releases v1.2.0](#releases-v120)
  * [Releases v1.1.0](#releases-v110)
  * [Initial Releases v1.0.0](#Initial-Releases-v100)

---
---

## Changelog

### Releases v1.2.2

1. Use `float` for `DutyCycle` and `Freq`, `uint32_t` for `period`. 
2. Optimize code by not calculation in ISR

### Releases v1.2.1

1. DutyCycle to be optionally updated at the end current PWM period instead of immediately. Check [DutyCycle to be updated at the end current PWM period #2](https://github.com/khoih-prog/ESP8266_PWM/issues/2)

### Releases v1.2.0

1. Fix `multiple-definitions` linker error. Drop `src_cpp` and `src_h` directories
2. Add example [multiFileProject](examples/multiFileProject) to demo for multiple-file project
3. Improve accuracy by using `double`, instead of `uint32_t` for `dutycycle`, `period`
4. Optimize library code by using `reference-passing` instead of `value-passing`
5. Update examples accordingly

### Releases v1.1.0

1. Add functions to modify PWM settings on-the-fly
2. Add example to demo how to modify PWM settings on-the-fly

### Initial Releases v1.0.0

1. Initial coding to support **AVR boards, such as Mega-2560, UNO,Nano, Leonardo, etc.**, etc. using AVR core

2. The hybrid ISR-based PWM channels can generate from very low (much less than 1Hz) to highest PWM frequencies up to 500Hz with acceptable accuracy.

---
---

## Copyright

Copyright 2021- Khoi Hoang


