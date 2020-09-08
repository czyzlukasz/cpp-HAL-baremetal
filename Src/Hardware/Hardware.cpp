#include <Hardware.hpp>

extern "C" {
uint32_t SystemCoreClock = 8e6;
}

void Hardware::enableGpio(GPIO_TypeDef* gpio, uint32_t pin, Gpio::Direction direction, Gpio::Pull pull) {

    GPIO_InitTypeDef initTypeDef;
    initTypeDef.Pin = pin;
    initTypeDef.Mode = direction == Gpio::Direction::Output ? GPIO_MODE_OUTPUT_PP : GPIO_MODE_INPUT;
    initTypeDef.Pull = static_cast<uint32_t>(pull);
    initTypeDef.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(gpio, &initTypeDef);
}

void Hardware::toggle(GPIO_TypeDef* gpio, uint32_t pin) {
    HAL_GPIO_TogglePin(gpio, pin);
}

void Hardware::configureClocks() {
    // Systick is executed once every 1ms
    SysTick_Config(SystemCoreClock / 1000);

    // Enable clocks for GPIOs
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
}
