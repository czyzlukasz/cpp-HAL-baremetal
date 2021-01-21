#include <Hardware.hpp>
#include <event_groups.h>

std::array<Uart::State, 2> Hardware::uartStates;
std::array<I2C::State, 2> Hardware::i2cStates;
std::array<SPI::State, 1> Hardware::spiState;
std::array<CAN::State, 1> Hardware::canState;


uint32_t CAN::RxMessage::getId() const {
    return header.ExtId;
}

CAN::TxMessage::TxMessage(uint32_t id) {
    header.ExtId = id;
    header.IDE = CAN_ID_EXT;
    header.RTR = CAN_RTR_DATA;
    header.DLC = payload.size();
}

void Hardware::enableGpio(GPIO_TypeDef* gpio, uint32_t pin, Gpio::Mode direction, Gpio::Pull pull) {

    GPIO_InitTypeDef initTypeDef;
    initTypeDef.Pin = pin;
    switch (direction) {
        case Gpio::Mode::Input:
            initTypeDef.Mode = GPIO_MODE_INPUT;
            break;
        case Gpio::Mode::Output:
            initTypeDef.Mode = GPIO_MODE_OUTPUT_PP;
            break;
        case Gpio::Mode::AlternateInput:
            initTypeDef.Mode = GPIO_MODE_AF_INPUT;
            break;
        case Gpio::Mode::AlternatePP:
            initTypeDef.Mode = GPIO_MODE_AF_PP;
            break;
        case Gpio::Mode::AlternateOD:
            initTypeDef.Mode = GPIO_MODE_AF_OD;
            break;
    }
    initTypeDef.Pull = static_cast<uint32_t>(pull);
    initTypeDef.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(gpio, &initTypeDef);
}

void Hardware::toggle(GPIO_TypeDef* gpio, uint32_t pin) {
    HAL_GPIO_TogglePin(gpio, pin);
}

void Hardware::configureClocks() {
    RCC_ClkInitTypeDef clkInitStruct;
    clkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
    clkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    HAL_RCC_ClockConfig(&clkInitStruct, FLASH_LATENCY_0);
    // Systick is executed once every 1ms
    SysTick_Config(SystemCoreClock / 1000);

    // Enable clocks for GPIOs
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
}

void Hardware::initializeUart(Uart::Uart id, uint32_t baudRate) {
    switch (id) {
        case Uart::Uart::UART_1:
            // Enable clocks
            __HAL_RCC_GPIOA_CLK_ENABLE();
            __HAL_RCC_USART1_CLK_ENABLE();
            // Set GPIO
            enableGpio(GPIOA, GPIO_PIN_9, Gpio::Mode::AlternatePP, Gpio::Pull::NoPull);  // TX1
            enableGpio(GPIOA, GPIO_PIN_10, Gpio::Mode::AlternateInput, Gpio::Pull::Pullup);  // RX1
            // Enable interrupts with low priority
            HAL_NVIC_SetPriority(USART1_IRQn, 10, 0);
            HAL_NVIC_EnableIRQ(USART1_IRQn);
            break;
        case Uart::Uart::UART_2:
            // Enable clocks
            __HAL_RCC_GPIOA_CLK_ENABLE();
            __HAL_RCC_USART2_CLK_ENABLE();
            // Set GPIO
            enableGpio(GPIOA, GPIO_PIN_2, Gpio::Mode::AlternatePP, Gpio::Pull::NoPull);  // TX2
            enableGpio(GPIOA, GPIO_PIN_3, Gpio::Mode::AlternateInput, Gpio::Pull::Pullup);  // RX2
            // Enable interrupts with low priority
            HAL_NVIC_SetPriority(USART2_IRQn, 10, 0);
            HAL_NVIC_EnableIRQ(USART2_IRQn);
            break;
    }

    Uart::State& state = getUartState(id);
    state.handle.Instance = id == Uart::Uart::UART_1 ? USART1 : USART2;
    state.handle.Init.BaudRate = baudRate;
    state.handle.Init.WordLength = UART_WORDLENGTH_8B;
    state.handle.Init.Parity = UART_PARITY_NONE;
    state.handle.Init.StopBits = UART_STOPBITS_1;
    state.handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    state.handle.Init.OverSampling = UART_OVERSAMPLING_16;
    state.handle.Init.Mode = UART_MODE_TX_RX;

    // Set registers with prepared data
    HAL_UART_Init(&state.handle);

    // Enable interrupts
    __HAL_UART_ENABLE_IT(&state.handle, UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(&state.handle, UART_IT_TC);

    // Clear bits
    state.txRxState = xEventGroupCreate();
    xEventGroupClearBits(state.txRxState, Uart::State::rxBit | Uart::State::txBit);
}

void Hardware::uartSend(Uart::Uart id, uint8_t *data, size_t numOfBytes) {
    Uart::State& state = getUartState(id);
    // Check if event group was created
    if(state.txRxState) {
        // Check if there is no transmission
        if((xEventGroupGetBits(state.txRxState) & Uart::State::txBit) == 0) {
            // If UART is not busy, transmit and set TX flag to busy
            HAL_UART_Transmit_IT(&state.handle, data, numOfBytes);
            xEventGroupSetBits(state.txRxState, Uart::State::txBit);
        }
    }
}

void Hardware::uartReceive(Uart::Uart id, uint8_t *data, size_t numOfBytes) {
    Uart::State& state = getUartState(id);
    // Check if event group was created
    if(state.txRxState) {
        // Check if there is no transmission
        if((xEventGroupGetBits(state.txRxState) & Uart::State::rxBit) == 0) {
            // If UART is not busy, transmit and set RX flag to busy
            HAL_UART_Receive_IT(&state.handle, data, numOfBytes);
            xEventGroupSetBits(state.txRxState, Uart::State::rxBit);
        }
    }
}

bool Hardware::isUartTxComplete(Uart::Uart id) {
    const Uart::State& state = getUartState(id);
    return (xEventGroupGetBits(state.txRxState) & Uart::State::txBit) == 0;
}

bool Hardware::isUartRxComplete(Uart::Uart id) {
    const Uart::State& state = getUartState(id);
    return (xEventGroupGetBits(state.txRxState) & Uart::State::rxBit) == 0;
}

void Hardware::abortUartTx(Uart::Uart id) {
    HAL_UART_AbortTransmit_IT(&getUartState(id).handle);
    xEventGroupClearBits(getUartState(id).txRxState, Uart::State::txBit);
}

void Hardware::abortUartRx(Uart::Uart id) {
    HAL_UART_AbortReceive_IT(&getUartState(id).handle);
    xEventGroupClearBits(getUartState(id).txRxState, Uart::State::rxBit);
}

Uart::State& Hardware::getUartState(Uart::Uart id) {
    switch (id) {
        case Uart::Uart::UART_1:
            return uartStates[0];
        case Uart::Uart::UART_2:
            return uartStates[1];
    }
    // Give up and stay here
    return getUartState(id);
}

void Hardware::initializeI2C(I2C::I2C id, uint32_t address, uint32_t speed) {
    switch (id) {
        case I2C::I2C::I2C_1:
            // Enable clocks
            __HAL_RCC_GPIOB_CLK_ENABLE();
            __HAL_RCC_I2C1_CLK_ENABLE();
            // Set GPIO
            enableGpio(GPIOB, GPIO_PIN_6, Gpio::Mode::AlternateOD, Gpio::Pull::Pullup);  // SCL
            enableGpio(GPIOB, GPIO_PIN_7, Gpio::Mode::AlternateOD, Gpio::Pull::Pullup);  // SDA
            // Enable interrupts with high priority due to silicon limitation (UM1850 p. 259)
            HAL_NVIC_SetPriority(I2C1_EV_IRQn, 1, 1);
            HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
            HAL_NVIC_SetPriority(I2C1_ER_IRQn, 1, 1);
            HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
            break;
        case I2C::I2C::I2C_2:
            break;
    }

    I2C::State& state = getI2CState(id);

    state.handle.Instance = I2C1;
    state.handle.Init.ClockSpeed = speed;
    state.handle.Init.DutyCycle = I2C_DUTYCYCLE_2;
    state.handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    state.handle.Init.OwnAddress1 = address;
    state.handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    state.handle.Init.OwnAddress2 = 0;
    state.handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    state.handle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    // Set registers with prepared data
    HAL_I2C_Init(&state.handle);

    // Enable interrupts
    __HAL_I2C_ENABLE_IT(&state.handle, I2C_IT_BUF);
    __HAL_I2C_ENABLE_IT(&state.handle, I2C_IT_EVT);
    __HAL_I2C_ENABLE_IT(&state.handle, I2C_IT_ERR);

    // Clear bits
    state.txRxState = xEventGroupCreate();
    xEventGroupClearBits(state.txRxState, I2C::State::rxBit | I2C::State::txBit);
}

void Hardware::i2cSendMaster(I2C::I2C id, uint16_t address, uint8_t *data, size_t numOfBytes) {
    I2C::State& state = getI2CState(id);
    // Check if event group was created
    if(state.txRxState) {
        // Check if there is no transmission
        if((xEventGroupGetBits(state.txRxState) & I2C::State::txBit) == 0) {
            // If I2C is not busy, transmit and set TX flag to busy
            HAL_I2C_Master_Transmit_IT(&state.handle, address, data, numOfBytes);
            xEventGroupSetBits(state.txRxState, I2C::State::txBit);
        }
    }
}

void Hardware::i2cReceiveMaster(I2C::I2C id, uint16_t address, uint8_t *data, size_t numOfBytes) {
    I2C::State& state = getI2CState(id);
    // Check if event group was created
    if(state.txRxState) {
        // Check if there is no transmission
        if((xEventGroupGetBits(state.txRxState) & I2C::State::rxBit) == 0) {
            // If I2C is not busy, transmit and set RX flag to busy
            HAL_I2C_Master_Transmit_IT(&state.handle, address, data, numOfBytes);
            xEventGroupSetBits(state.txRxState, I2C::State::rxBit);
        }
    }
}

I2C::State &Hardware::getI2CState(I2C::I2C id) {
    switch (id) {
        case I2C::I2C::I2C_1:
            return i2cStates[0];
        case I2C::I2C::I2C_2:
            return i2cStates[1];
    }
    // Give up and stay here
    return getI2CState(id);
}

void Hardware::initializeSpi() {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_SPI1_CLK_ENABLE();
    enableGpio(GPIOA, GPIO_PIN_5, Gpio::Mode::AlternatePP, Gpio::Pull::NoPull);  // SCK
    enableGpio(GPIOA, GPIO_PIN_6, Gpio::Mode::AlternateInput, Gpio::Pull::NoPull);  // MISO
    enableGpio(GPIOA, GPIO_PIN_7, Gpio::Mode::AlternatePP, Gpio::Pull::NoPull);  // MOSI
    HAL_NVIC_SetPriority(SPI1_IRQn, 5, 5);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);

    SPI_HandleTypeDef& handle = spiState.at(0).handle;
    handle.Instance = SPI1;
    handle.Init.Mode = SPI_MODE_MASTER;
    handle.Init.Direction = SPI_DIRECTION_1LINE;
    handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    handle.Init.CLKPhase = SPI_PHASE_2EDGE;
    handle.Init.CLKPolarity = SPI_POLARITY_HIGH;
    handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    handle.Init.DataSize = SPI_DATASIZE_8BIT;
    handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    handle.Init.NSS = SPI_NSS_SOFT;
    handle.Init.TIMode = SPI_TIMODE_DISABLE;

    HAL_SPI_Init(&handle);

    __HAL_SPI_ENABLE_IT(&handle, SPI_IT_TXE);
    __HAL_SPI_ENABLE_IT(&handle, SPI_IT_RXNE);
    __HAL_SPI_ENABLE_IT(&handle, SPI_IT_ERR);

    // Clear bits
    spiState.at(0).txRxState = xEventGroupCreate();
    xEventGroupClearBits(spiState.at(0).txRxState, SPI::State::rxBit | SPI::State::txBit);
}

void Hardware::spiSend(uint8_t *data, size_t numOfBytes) {
    SPI::State& state = getSpiState();
    // Check if event group was created
    if(state.txRxState) {
        // Check if there is no transmission
        if((xEventGroupGetBits(state.txRxState) & SPI::State::txBit) == 0) {
            // If UART is not busy, transmit and set TX flag to busy
            HAL_SPI_Transmit_IT(&state.handle, data, numOfBytes);
            xEventGroupSetBits(state.txRxState, Uart::State::txBit);
        }
    }
}

void Hardware::spiReceive(uint8_t *data, size_t numOfBytes) {
    SPI::State& state = getSpiState();
    // Check if event group was created
    if(state.txRxState) {
        // Check if there is no transmission
        if((xEventGroupGetBits(state.txRxState) & SPI::State::rxBit) == 0) {
            // If UART is not busy, transmit and set RX flag to busy
            HAL_SPI_Receive_IT(&state.handle, data, numOfBytes);
            xEventGroupSetBits(state.txRxState, SPI::State::rxBit);
        }
    }
}

SPI::State &Hardware::getSpiState() {
    return spiState.at(0);
}

void Hardware::initializeCan() {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_CAN1_CLK_ENABLE();
    enableGpio(GPIOA, GPIO_PIN_11, Gpio::Mode::AlternateOD, Gpio::Pull::NoPull); // RX
    enableGpio(GPIOA, GPIO_PIN_12, Gpio::Mode::AlternateOD, Gpio::Pull::NoPull); // TX
    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 5, 5);
    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
    HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 5, 5);
    HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);

    CAN_HandleTypeDef& handle = getCanState().handle;

    handle.Instance = CAN1;
    handle.Init.Prescaler = 1;
    handle.Init.Mode = CAN_MODE_NORMAL;
    handle.Init.SyncJumpWidth = CAN_SJW_1TQ;
    handle.Init.TimeSeg1 = CAN_BS1_6TQ;
    handle.Init.TimeSeg2 = CAN_BS2_1TQ;
    handle.Init.TimeTriggeredMode = DISABLE;
    handle.Init.AutoBusOff = DISABLE;
    handle.Init.AutoWakeUp = DISABLE;
    handle.Init.AutoRetransmission = ENABLE;
    handle.Init.ReceiveFifoLocked = DISABLE;
    handle.Init.TransmitFifoPriority = DISABLE;

    HAL_CAN_Init(&handle);

    HAL_CAN_Start(&handle);
}

bool Hardware::isAnyTxMailboxFree() {
    return HAL_CAN_GetTxMailboxesFreeLevel(&getCanState().handle) > 0;
}

bool Hardware::isAnyRxMessagePending() {
    return HAL_CAN_GetRxFifoFillLevel(&getCanState().handle, CAN_RX_FIFO0) || HAL_CAN_GetRxFifoFillLevel(&getCanState().handle, CAN_RX_FIFO1);
}

std::optional<CAN::RxMessage> Hardware::receiveCanMessage() {
    if(HAL_CAN_GetRxFifoFillLevel(&getCanState().handle, CAN_RX_FIFO0)){    // There is message in FIFO0
        CAN::RxMessage message{};
        HAL_CAN_GetRxMessage(&getCanState().handle, CAN_RX_FIFO0, &message.header, message.payload.data());
        return message;
    }
    else if(HAL_CAN_GetRxFifoFillLevel(&getCanState().handle, CAN_RX_FIFO1)){   // There is message in FIFO1
        CAN::RxMessage message{};
        HAL_CAN_GetRxMessage(&getCanState().handle, CAN_RX_FIFO1, &message.header, message.payload.data());
        return message;
    }
    else{   // No message waiting
        return std::nullopt;
    }
}

void Hardware::sendCanMessage(CAN::TxMessage &message) {
    [[maybe_unused]] uint32_t usedMailbox;
    if(isAnyTxMailboxFree()) {
        HAL_CAN_AddTxMessage(&getCanState().handle, &message.header, message.payload.data(), &usedMailbox);
    }
}

CAN::State& Hardware::getCanState() {
    return canState.at(0);
}

// Handlers for default HAL UART callbacks

void USART1_IRQHandler() {
    HAL_UART_IRQHandler(&Hardware::getUartState(Uart::Uart::UART_1).handle);
}

void USART2_IRQHandler() {
    HAL_UART_IRQHandler(&Hardware::getUartState(Uart::Uart::UART_2).handle);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if(huart->Instance == USART1){
        if(auto* eventGroup = Hardware::getUartState(Uart::Uart::UART_1).txRxState) {
            xEventGroupClearBitsFromISR(eventGroup, Uart::State::txBit);
        }
    }
    else if(huart->Instance == USART2){
        if(auto* eventGroup = Hardware::getUartState(Uart::Uart::UART_2).txRxState) {
            xEventGroupClearBitsFromISR(eventGroup, Uart::State::txBit);
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if(huart->Instance == USART1){
        if(auto* eventGroup = Hardware::getUartState(Uart::Uart::UART_1).txRxState) {
            xEventGroupClearBitsFromISR(eventGroup, Uart::State::rxBit);
        }
    }
    if(huart->Instance == USART2){
        if(auto* eventGroup = Hardware::getUartState(Uart::Uart::UART_1).txRxState) {
            xEventGroupClearBitsFromISR(eventGroup, Uart::State::rxBit);
        }
    }
}

// Handlers for I2C transmission

void I2C1_EV_IRQHandler(){
    HAL_I2C_EV_IRQHandler(&Hardware::getI2CState(I2C::I2C::I2C_1).handle);
}

void I2C1_ER_IRQHandler(){
    HAL_I2C_ER_IRQHandler(&Hardware::getI2CState(I2C::I2C::I2C_1).handle);
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c){
    if(hi2c->Instance == I2C1){
        if(auto* eventGroup = Hardware::getI2CState(I2C::I2C::I2C_1).txRxState) {
            xEventGroupClearBitsFromISR(eventGroup, Uart::State::txBit);
        }
    }
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c){
    if(hi2c->Instance == I2C1){
        if(auto* eventGroup = Hardware::getI2CState(I2C::I2C::I2C_1).txRxState) {
            xEventGroupClearBitsFromISR(eventGroup, Uart::State::rxBit);
        }
    }
}

// Handlers for SPI transmission

void SPI1_IRQHandler(){
    HAL_SPI_IRQHandler(&Hardware::getSpiState().handle);
}

void HAL_SPI_TxCpltCallback([[maybe_unused]] SPI_HandleTypeDef *hspi){
    if(auto* eventGroup = Hardware::getSpiState().txRxState) {
        xEventGroupClearBitsFromISR(eventGroup, Uart::State::txBit);
    }
}
void HAL_SPI_RxCpltCallback([[maybe_unused]] SPI_HandleTypeDef *hspi){
    if(auto* eventGroup = Hardware::getSpiState().txRxState) {
        xEventGroupClearBitsFromISR(eventGroup, Uart::State::rxBit);
    }
}

void HAL_SPI_TxHalfCpltCallback([[maybe_unused]] SPI_HandleTypeDef *hspi){
    if(auto* eventGroup = Hardware::getSpiState().txRxState) {
        xEventGroupClearBitsFromISR(eventGroup, Uart::State::rxBit);
    }
}

void HAL_SPI_RxHalfCpltCallback([[maybe_unused]] SPI_HandleTypeDef *hspi){
    if(auto* eventGroup = Hardware::getSpiState().txRxState) {
        xEventGroupClearBitsFromISR(eventGroup, Uart::State::rxBit);
    }
}

void HAL_SPI_ErrorCallback([[maybe_unused]] SPI_HandleTypeDef *hspi){
    if(auto* eventGroup = Hardware::getSpiState().txRxState) {
        xEventGroupClearBitsFromISR(eventGroup, Uart::State::rxBit);
    }
}

// Handlers for CAN transmission

void CAN1_IRQHandler(){
    HAL_CAN_IRQHandler(&Hardware::getCanState().handle);
}
