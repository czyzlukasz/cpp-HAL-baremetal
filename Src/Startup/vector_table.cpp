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
};
