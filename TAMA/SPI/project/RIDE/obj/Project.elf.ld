
/*
!!! Primary linker script file generated automatically !!!
This file contains the input and output files names
It also includes the secondary/main linker script that is specified in the Ride options (default or custom)
*/

SEARCH_DIR(".")
SEARCH_DIR("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj")
SEARCH_DIR("C:\Program Files (x86)\Raisonance\Ride\Lib\ARM")
STARTUP("startup_stm32f10x_md.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\main.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_it.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_adc.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_bkp.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_can.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_crc.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_dac.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_dbgmcu.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_dma.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_exti.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_flash.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_fsmc.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_gpio.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_i2c.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_iwdg.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_lib.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_nvic.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_pwr.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_rcc.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_rtc.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_sdio.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_spi.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_systick.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_tim.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_usart.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\stm32f10x_wwdg.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\Util.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\draw.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\lcd.o")
INPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\buzzer.o")
EXTERN( __io_putchar )
EXTERN( _write )
GROUP(
"STM32x_io_putchar_thumb.a"
"std_sbrk_thumb.lib"
"smallprintf_thumb.a"
"libc.a"
"libm.a"

)
INCLUDE "C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\STM32F103_64K_20K_FLASH.ld"
OUTPUT("C:\Users\fv\Desktop\STM32_LAMEC_CAN\TAMA\SPI\project\RIDE\obj\Project.elf")
