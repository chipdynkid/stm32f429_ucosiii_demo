# stm32f429_ucosiii_demo
μC/OS-III Learning & STM32F429 Porting
This repository documents my learning journey with μC/OS-III (a real-time operating system) and provides practical, bare-metal examples for the STM32F429IGT6 microcontroller. It’s designed to serve as a reference for developers or enthusiasts who want to learn RTOS porting and application development on STM32 platforms.
## 1. Repository Overview
All code in this repo follows these technical foundations:
### 1.1 Target Hardware: STM32F429IGT6 microcontroller (focus on core peripherals and RTOS integration)
### 1.2 Software Stack: STM32 HAL (Hardware Abstraction Layer) library (for low-level hardware control)
### 1.3 SRTOS: μC/OS-III (source code ported and adapted to the STM32F429 platform)
### 1.4 Development Approach: Bare-metal environment (no pre-installed RTOS; porting and configuration done from scratch)
The examples cover key μC/OS-III concepts (e.g., task management, semaphores, mutexes, timers) paired with STM32 hardware interactions (e.g., GPIO, UART, timers) to bridge theoretical RTOS knowledge with practical embedded development.
## 2. Learning Resources
My study of μC/OS-III and STM32 porting relies on the following authoritative references:
### 2.1 Official μC/OS-III Reference Manual (from Micrium, the developer of μC/OS-III)
### 2.2 Zhengdian Atom’s μC/OS-III Development Guide & Example Code (practical STM32-specific RTOS tutorials)
### 2.3 Analysis and Application Development of the μC/OS-III Kernel (a book focusing on μC/OS-III internal mechanisms and real-world use cases)
## 3. Notes for Users
All code is tested on STM32F429IGT6-based development boards. Adjustments to hardware pins or HAL configurations may be needed for other STM32F4 variants.
If you find bugs, have optimization suggestions, or want to add new μC/OS-III examples, feel free to open an issue or submit a pull request (PR).

