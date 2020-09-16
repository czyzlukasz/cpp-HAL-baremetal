typedef void (*ptr_func_t)();

extern "C" void __stop() { while (true); }

__attribute__((weak, alias("__stop"))) void resetHandler();
__attribute__((weak, alias("__stop"))) void NMI_handler();
__attribute__((weak, alias("__stop"))) void hardfault();
__attribute__((weak, alias("__stop"))) void MEMMANAGE_handler();
__attribute__((weak, alias("__stop"))) void BUSFAULT_handler();
__attribute__((weak, alias("__stop"))) void USAGEFAULT_handler();
__attribute__((weak, alias("__stop"))) void svcHandler();
__attribute__((weak, alias("__stop"))) void DEBUGMONITOR_handler();
__attribute__((weak, alias("__stop"))) void pendSvcHandler();
__attribute__((weak, alias("__stop"))) void systick();
__attribute__((weak, alias("__stop"))) void WWDG_IRQHandler();
__attribute__((weak, alias("__stop"))) void PVD_IRQHandler();
__attribute__((weak, alias("__stop"))) void TAMPER_IRQHandler();
__attribute__((weak, alias("__stop"))) void RTC_IRQHandler();
__attribute__((weak, alias("__stop"))) void FLASH_IRQHandler();
__attribute__((weak, alias("__stop"))) void RCC_IRQHandler();
__attribute__((weak, alias("__stop"))) void EXTI0_IRQHandler();
__attribute__((weak, alias("__stop"))) void EXTI1_IRQHandler();
__attribute__((weak, alias("__stop"))) void EXTI2_IRQHandler();
__attribute__((weak, alias("__stop"))) void EXTI3_IRQHandler();
__attribute__((weak, alias("__stop"))) void EXTI4_IRQHandler();
__attribute__((weak, alias("__stop"))) void DMA1_Channel1_IRQHandler();
__attribute__((weak, alias("__stop"))) void DMA1_Channel2_IRQHandler();
__attribute__((weak, alias("__stop"))) void DMA1_Channel3_IRQHandler();
__attribute__((weak, alias("__stop"))) void DMA1_Channel4_IRQHandler();
__attribute__((weak, alias("__stop"))) void DMA1_Channel5_IRQHandler();
__attribute__((weak, alias("__stop"))) void DMA1_Channel6_IRQHandler();
__attribute__((weak, alias("__stop"))) void DMA1_Channel7_IRQHandler();
__attribute__((weak, alias("__stop"))) void ADC1_2_IRQHandler();
__attribute__((weak, alias("__stop"))) void USB_HP_CAN1_TX_IRQHandler();
__attribute__((weak, alias("__stop"))) void USB_LP_CAN1_RX0_IRQHandler();
__attribute__((weak, alias("__stop"))) void CAN1_RX1_IRQHandler();
__attribute__((weak, alias("__stop"))) void CAN1_SCE_IRQHandler();
__attribute__((weak, alias("__stop"))) void EXTI9_5_IRQHandler();
__attribute__((weak, alias("__stop"))) void TIM1_BRK_IRQHandler();
__attribute__((weak, alias("__stop"))) void TIM1_UP_IRQHandler();
__attribute__((weak, alias("__stop"))) void TIM1_TRG_COM_IRQHandler();
__attribute__((weak, alias("__stop"))) void TIM1_CC_IRQHandler();
__attribute__((weak, alias("__stop"))) void TIM2_IRQHandler();
__attribute__((weak, alias("__stop"))) void TIM3_IRQHandler();
__attribute__((weak, alias("__stop"))) void TIM4_IRQHandler();
__attribute__((weak, alias("__stop"))) void I2C1_EV_IRQHandler();
__attribute__((weak, alias("__stop"))) void I2C1_ER_IRQHandler();
__attribute__((weak, alias("__stop"))) void I2C2_EV_IRQHandler();
__attribute__((weak, alias("__stop"))) void I2C2_ER_IRQHandler();
__attribute__((weak, alias("__stop"))) void SPI1_IRQHandler();
__attribute__((weak, alias("__stop"))) void SPI2_IRQHandler();
__attribute__((weak, alias("__stop"))) void USART1_IRQHandler();
__attribute__((weak, alias("__stop"))) void USART2_IRQHandler();
__attribute__((weak, alias("__stop"))) void USART3_IRQHandler();
__attribute__((weak, alias("__stop"))) void EXTI15_10_IRQHandler();
__attribute__((weak, alias("__stop"))) void RTC_Alarm_IRQHandler();
__attribute__((weak, alias("__stop"))) void USBWakeUp_IRQHandler();
__attribute__((weak, alias("__stop"))) void TIM8_BRK_IRQHandler();
__attribute__((weak, alias("__stop"))) void TIM8_UP_IRQHandler();
__attribute__((weak, alias("__stop"))) void TIM8_TRG_COM_IRQHandler();
__attribute__((weak, alias("__stop"))) void TIM8_CC_IRQHandler();
__attribute__((weak, alias("__stop"))) void ADC3_IRQHandler();
__attribute__((weak, alias("__stop"))) void FSMC_IRQHandler();
__attribute__((weak, alias("__stop"))) void SDIO_IRQHandler();
__attribute__((weak, alias("__stop"))) void TIM5_IRQHandler();
__attribute__((weak, alias("__stop"))) void SPI3_IRQHandler();
__attribute__((weak, alias("__stop"))) void UART4_IRQHandler();
__attribute__((weak, alias("__stop"))) void UART5_IRQHandler();
__attribute__((weak, alias("__stop"))) void TIM6_IRQHandler();
__attribute__((weak, alias("__stop"))) void TIM7_IRQHandler();
__attribute__((weak, alias("__stop"))) void DMA2_Channel1_IRQHandler();
__attribute__((weak, alias("__stop"))) void DMA2_Channel2_IRQHandler();
__attribute__((weak, alias("__stop"))) void DMA2_Channel3_IRQHandler();
__attribute__((weak, alias("__stop"))) void DMA2_Channel4_5_IRQHandler();


__attribute__((weak, alias("__stop"))) void DUMMY_handler();

__attribute__((section(".vectors"), used)) ptr_func_t __isr_vectors[] = {
    resetHandler,
    NMI_handler,
    hardfault,
    MEMMANAGE_handler,
    BUSFAULT_handler,
    USAGEFAULT_handler,
    DUMMY_handler,
    DUMMY_handler,
    DUMMY_handler,
    DUMMY_handler,
    svcHandler,
    DEBUGMONITOR_handler,
    DUMMY_handler,
    pendSvcHandler,
    systick,
    WWDG_IRQHandler,
    PVD_IRQHandler,
    TAMPER_IRQHandler,
    RTC_IRQHandler,
    FLASH_IRQHandler,
    RCC_IRQHandler,
    EXTI0_IRQHandler,
    EXTI1_IRQHandler,
    EXTI2_IRQHandler,
    EXTI3_IRQHandler,
    EXTI4_IRQHandler,
    DMA1_Channel1_IRQHandler,
    DMA1_Channel2_IRQHandler,
    DMA1_Channel3_IRQHandler,
    DMA1_Channel4_IRQHandler,
    DMA1_Channel5_IRQHandler,
    DMA1_Channel6_IRQHandler,
    DMA1_Channel7_IRQHandler,
    ADC1_2_IRQHandler,
    USB_HP_CAN1_TX_IRQHandler,
    USB_LP_CAN1_RX0_IRQHandler,
    CAN1_RX1_IRQHandler,
    CAN1_SCE_IRQHandler,
    EXTI9_5_IRQHandler,
    TIM1_BRK_IRQHandler,
    TIM1_UP_IRQHandler,
    TIM1_TRG_COM_IRQHandler,
    TIM1_CC_IRQHandler,
    TIM2_IRQHandler,
    TIM3_IRQHandler,
    TIM4_IRQHandler,
    I2C1_EV_IRQHandler,
    I2C1_ER_IRQHandler,
    I2C2_EV_IRQHandler,
    I2C2_ER_IRQHandler,
    SPI1_IRQHandler,
    SPI2_IRQHandler,
    USART1_IRQHandler,
    USART2_IRQHandler,
    USART3_IRQHandler,
    EXTI15_10_IRQHandler,
    RTC_Alarm_IRQHandler,
    USBWakeUp_IRQHandler,
    TIM8_BRK_IRQHandler,
    TIM8_UP_IRQHandler,
    TIM8_TRG_COM_IRQHandler,
    TIM8_CC_IRQHandler,
    ADC3_IRQHandler,
    FSMC_IRQHandler,
    SDIO_IRQHandler,
    TIM5_IRQHandler,
    SPI3_IRQHandler,
    UART4_IRQHandler,
    UART5_IRQHandler,
    TIM6_IRQHandler,
    TIM7_IRQHandler,
    DMA2_Channel1_IRQHandler,
    DMA2_Channel2_IRQHandler,
    DMA2_Channel3_IRQHandler,
    DMA2_Channel4_5_IRQHandler
};
