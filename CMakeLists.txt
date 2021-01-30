cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_SYSTEM_NAME Generic)

project(baremetal)

set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(LINKER_SCRIPT "Src/Startup/stm32f031x6.ld")

set(CPU_OPTIONS -mthumb -mcpu=cortex-m3)


include_directories(
    ${CMAKE_SOURCE_DIR}
)

link_directories(DCMAKE_SYSTEM_NAME
    ${CMAKE_SOURCE_DIR}
)

# End of boilerplate

add_compile_options(
    ${CPU_OPTIONS}
    -Werror
    -Wall
    -Wextra
    -pedantic
    -ffunction-sections
    -fdata-sections
    -Os
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSTM32F103x6 -Wl,-gc-sections")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSTM32F103x6 -Wl,-gc-sections")

include_directories(Cube/Drivers/STM32F1xx_HAL_Driver/Src)
include_directories(Cube/Drivers/STM32F1xx_HAL_Driver/Inc)
include_directories(Cube/Drivers/CMSIS/Device/ST/STM32F1xx/Include)
include_directories(Cube/Drivers/CMSIS/Include)
include_directories(Cube/Drivers/CMSIS/Include)

include_directories(Src/Hardware)
include_directories(Src/Tasks)

# FreeRTOS
include_directories(FreeRTOS-Kernel/include)
include_directories(FreeRTOS-Kernel/portable/GCC/ARM_CM3)
include_directories(FreeRTOS-Kernel/portable/MemMang)


add_executable(target
        # Starting procedure
        Src/Startup/main.cpp
        Src/Startup/vector_table.cpp

        # Hardware abstraction
        Src/Hardware/Hardware.cpp
        Src/Tasks/TaskManager.cpp

        # User code
        Src/Tasks/LedDebug.cpp

        # FreeRTOS
        FreeRTOS-Kernel/portable/MemMang/heap_4.c
        FreeRTOS-Kernel/portable/GCC/ARM_CM3/port.c
        FreeRTOS-Kernel/tasks.c
        FreeRTOS-Kernel/list.c
        FreeRTOS-Kernel/timers.c
        FreeRTOS-Kernel/queue.c
        FreeRTOS-Kernel/event_groups.c

        # HAL
        Cube/Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c
        Cube/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c
        Cube/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c
        Cube/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c
        Cube/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_usart.c
        Cube/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2c.c
        Cube/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c
        Cube/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c
        Cube/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c
        Cube/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_can.c
        Cube/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c)


target_link_libraries(target ${CPU_OPTIONS} -T${LINKER_SCRIPT} -nostartfiles -specs=nano.specs -specs=nosys.specs)

# Start of boilerplate
set_property(TARGET target PROPERTY LINK_DEPENDS ${CMAKE_SOURCE_DIR}/${LINKER_SCRIPT})

set(CMAKE_EXE_LINKER_FLAGS "" CACHE INTERNAL "")
