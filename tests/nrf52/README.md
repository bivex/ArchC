# ArchC Nordic Semiconductor (nRF52 / nRF53 / nRF91) Architecture Model & Verification Suite

## 🔋 Nordic Semiconductor — Короли автономности и Bluetooth (BLE)

- **Архитектура:** 32-битный ARM Cortex-M4F (ARMv7E-M Thumb-2, `ac_wordsize 32`, `ac_fetchsize 16`, Little-Endian).
- **Популярные чипы:**
  - **nRF52840 / nRF52832:** Флагманы Bluetooth Low Energy (BLE 5.0), Thread, Zigbee, ANT, 2.4 GHz RF.
  - **nRF5340:** Двухъядерный чип (Application Core Cortex-M33 + Network Core Cortex-M33) для аудиоустройств нового поколения (LE Audio).
  - **nRF9160:** Сверхмалопотребляющий SiP со встроенным модемом LTE-M / NB-IoT и GPS.
- **Главный козырь: Сверхнизкое энергопотребление (Ultra-Low Power)**:
  - В отличие от ESP32, расходующего много энергии при пробуждении, чипы Nordic на одной маленькой батарейке-таблетке **CR2032 работают 3–5 ЛЕТ!**
- **Где стоят:** Беспроводные мыши/клавиатуры (Logitech), фитнес-браслеты, медицинские глюкометры и пульсоксиметры, метки-трекеры (аналоги Apple AirTag), умные дверные замки.

---

## ⚙️ Особенности модели ArchC nRF52840:
- **Ядро:** 32-битный ARM Cortex-M4F с набором инструкций Thumb/Thumb-2.
- **Регистры:** 16 регистров общего назначения (`R0`–`R15`, `R13`=SP, `R14`=LR, `R15`=PC), регистры состояния `xPSR`, `primask`, `control`, `event_reg`.
- **Инструкции энергосбережения:** Аппаратная симуляция инструкций `WFE` (Wait For Event), `SEV` (Send Event) и `WFI` (Wait For Interrupt).
- **Карта памяти:**
  - 1 МБ Flash-памяти программ (`0x00000000`)
  - 256 КБ SRAM с поддержкой EasyDMA (`0x20000000`)
  - Пространство периферии и 2.4GHz BLE Radio (`0x40000000`)
- **Загрузчик ELF:** нативная поддержка Little-Endian ELF32 ARM (`EM_ARM = 40 = 0x28`, `EF_ARM_EABI_VER5`).
