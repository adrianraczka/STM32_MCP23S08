cmake_minimum_required(VERSION 3.15.3)

project(nucleo-F446RE)

enable_language(CXX C ASM)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(STM32CUBEMX_GENERATED_FILES

		CubeMX_files/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c
		CubeMX_files/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
		CubeMX_files/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
		CubeMX_files/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
		CubeMX_files/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
		CubeMX_files/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
		CubeMX_files/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
		CubeMX_files/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
		CubeMX_files/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
		CubeMX_files/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
		CubeMX_files/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
		CubeMX_files/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
		CubeMX_files/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
		CubeMX_files/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
		CubeMX_files/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
		CubeMX_files/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
		CubeMX_files/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
		CubeMX_files/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
		CubeMX_files/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd.c
		CubeMX_files/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c
		CubeMX_files/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c

		CubeMX_files/Inc/main.h
		CubeMX_files/Inc/MCP23S08.h
		CubeMX_files/Inc/gpio.h
		CubeMX_files/Inc/spi.h
		CubeMX_files/Inc/usart.h
		CubeMX_files/Inc/stm32f4xx_hal_conf.h
		CubeMX_files/Inc/stm32f4xx_it.h

		CubeMX_files/Src/main.c
		CubeMX_files/Src/MCP23S08.c
		CubeMX_files/Src/gpio.c
		CubeMX_files/Src/spi.c
		CubeMX_files/Src/usart.c
		CubeMX_files/Src/stm32f4xx_hal_msp.c
		CubeMX_files/Src/stm32f4xx_it.c
		CubeMX_files/Src/system_stm32f4xx.c

		CubeMX_files/startup_stm32f446xx.s)

set(EXECUTABLE ${PROJECT_NAME}.out)

add_executable(${EXECUTABLE} ${STM32CUBEMX_GENERATED_FILES}
		)

target_compile_definitions(${EXECUTABLE} PRIVATE
		-DUSE_HAL_DRIVER
		-DSTM32F446xx
		)

target_include_directories(${EXECUTABLE} PRIVATE
		CubeMX_files/Inc
		CubeMX_files/Drivers/STM32F4xx_HAL_Driver/Inc
		CubeMX_files/Drivers/CMSIS/Device/ST/STM32F4xx/Include
		CubeMX_files/Drivers/CMSIS/Include
		)

target_compile_options(${EXECUTABLE} PRIVATE
		-mcpu=cortex-m4
		-mthumb
		-mfpu=fpv4-sp-d16
		-mfloat-abi=hard


		-ffunction-sections
		-fdata-sections
		-fno-exceptions
		-Wall
		$<$<CONFIG:Debug>:-Og>
		)

target_link_options(${EXECUTABLE} PRIVATE
		-T${CMAKE_SOURCE_DIR}/CubeMX_files/STM32F446RETx_FLASH.ld
		-mcpu=cortex-m4
		-mthumb
		-mfpu=fpv4-sp-d16
		-mfloat-abi=hard
		-lc
		-lm
		-lnosys
		-specs=nosys.specs
		-u _printf_float

		-Wl,-Map=${PROJECT_NAME}.map,--cref
		-Wl,--gc-sections
		-Wl,--print-memory-usage
		)

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
		POST_BUILD
		COMMAND arm-none-eabi-size ${EXECUTABLE})

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
		POST_BUILD
		COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
		COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)