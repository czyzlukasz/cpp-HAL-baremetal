#include <Hardware.hpp>

extern "C" {
uint32_t SystemCoreClock = 8e6;
}

void Hardware::enableGpio() {
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef initTypeDef;
    initTypeDef.Pin = GPIO_PIN_13;
    initTypeDef.Mode = GPIO_MODE_OUTPUT_PP;
    initTypeDef.Pull = GPIO_NOPULL;
    initTypeDef.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &initTypeDef);
}

void Hardware::toggle() {
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}
