# ArchC STM32F103 (ARM Cortex-M3 / Blue Pill) Model & Verification Suite

This directory contains a complete processor model for the STM32F103 microcontroller powered by the 32-bit ARM Cortex-M3 (ARMv7-M) core.

## Features
- 32-bit ARMv7-M Thumb/Thumb-2 Architecture (`ac_wordsize 32`, `ac_fetchsize 16`, Little-Endian).
- 16 32-bit General Purpose Registers (`R0`–`R15`, `R13`=SP, `R14`=LR, `R15`=PC).
- Status Register `xPSR` (APSR flags `N`, `Z`, `C`, `V` + `T` Thumb bit).
- Memory Map:
  - 128 KB On-Chip Flash at `0x08000000`
  - 20 KB On-Chip SRAM at `0x20000000`
  - Peripherals and NVIC at `0x40000000` / `0xE000E000`
- Native Little-Endian ELF32 (`EM_ARM = 40 = 0x28`, `EF_ARM_EABI_VER5`) loader support.
