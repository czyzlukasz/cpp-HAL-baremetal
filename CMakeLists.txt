cmake_minimum_required(VERSION 3.15)

project(baremetal)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(LINKER_SCRIPT "Src/Startup/stm32f031x6.ld")

set(CPU_OPTIONS -mthumb -mcpu=cortex-m3)

set(CMAKE_C_COMPILER /usr/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/arm-none-eabi-g++)

add_compile_options(
    ${CPU_OPTIONS}
    -ffunction-sections
    -Werror
    -Wall
    -Wextra
    -pedantic
    -fno-exceptions
)

include_directories(
    ${CMAKE_SOURCE_DIR}
)

link_directories(
    ${CMAKE_SOURCE_DIR}
)

# End of boilerplate

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSTM32F103x6")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSTM32F103x6")
include_directories(Cube/Drivers/STM32F1xx_HAL_Driver/Src)
include_directories(Cube/Drivers/STM32F1xx_HAL_Driver/Inc)
include_directories(Cube/Drivers/CMSIS/Device/ST/STM32F1xx/Include)
include_directories(Cube/Drivers/CMSIS/Include)
include_directories(Cube/Drivers/CMSIS/Include)

include_directories(Src/Hardware)


add_executable(target
        Src/Startup/main.cpp
        Src/Startup/vector_table.cpp
        Src/Hardware/Hardware.cpp

        Cube/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c
        Cube/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c
        Cube/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c)


# Start of boilerplate

target_link_libraries(target ${CPU_OPTIONS} -T${LINKER_SCRIPT} -nostartfiles m -nostdlib)
set_property(TARGET target PROPERTY LINK_DEPENDS ${CMAKE_SOURCE_DIR}/${LINKER_SCRIPT})
