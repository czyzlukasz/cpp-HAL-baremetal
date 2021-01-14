#ifndef HARDWARE_HPP
#define HARDWARE_HPP
#include <stm32f1xx_hal.h>
#include <FreeRTOS.h>
#include <FreeRTOS-Kernel/include/event_groups.h>
#include <map>

// Goal of this abstraction is to isolate main code from hardware-specific code. Ideally, this code could use a stub and
// be compiled for Linux without any problem
namespace Gpio {
    enum class Mode {
        Input,
        Output,
        AlternateInput,
        AlternatePP,
        AlternateOD
    };

    enum class Pull {
        Pullup = 1,
        PullDown = 2,
        NoPull = 0
    };
}

namespace Uart {
    enum class Uart {
        UART_1,
        UART_2
    };

    struct State {
        EventGroupHandle_t txRxState;
        UART_HandleTypeDef handle;

        static constexpr size_t txBit = 1 << 0;
        static constexpr size_t rxBit = 1 << 1;
    };
}

namespace I2C {
    enum class I2C {
        I2C_1,
        I2C_2
    };
//
//    enum class SpeedMode {
//        STANDARD,   //100kHz
//        FAST,       //400kHz
//        FAST_PLUS,  //1MHz
//        HIGH_SPEED, //3.4MHz
//        ULTRA_FAST  //5MHz
//    };

    struct State {
        EventGroupHandle_t txRxState;
        I2C_HandleTypeDef handle;

        static constexpr size_t txBit = 1 << 0;
        static constexpr size_t rxBit = 1 << 1;
    };
}

struct Hardware {
    static void enableGpio(GPIO_TypeDef* gpio, uint32_t pin, Gpio::Mode direction, Gpio::Pull pull = Gpio::Pull::NoPull);
    static void toggle(GPIO_TypeDef* gpio, uint32_t pin);
    static void configureClocks();

    static void initializeUart(Uart::Uart id, uint32_t baudRate);
    static void uartSend(Uart::Uart id, uint8_t data[], size_t numOfBytes);
    static void uartReceive(Uart::Uart id, uint8_t data[], size_t numOfBytes);
    static bool isUartTxComplete(Uart::Uart id);
    static bool isUartRxComplete(Uart::Uart id);
    static void abortUartTx(Uart::Uart id);
    static void abortUartRx(Uart::Uart id);
    static Uart::State& getUartState(Uart::Uart id);

    static void initializeI2C(I2C::I2C id, uint32_t address, uint32_t speed);
    static void i2cSendMaster(I2C::I2C id, uint16_t address, uint8_t data[], size_t numOfBytes);
    static void i2cReceiveMaster(I2C::I2C id, uint16_t address, uint8_t data[], size_t numOfBytes);
    static I2C::State& getI2CState(I2C::I2C id);

private:
    static std::array<Uart::State, 2> uartStates;
    static std::array<I2C::State, 2> i2cStates;
};


#endif //HARDWARE_HPP
