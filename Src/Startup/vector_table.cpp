typedef void (*ptr_func_t)();

extern "C" void __stop() { while (true); }

__attribute__((weak, alias("__stop"))) void resetHandler();
__attribute__((weak, alias("__stop"))) void NMI_handler();
__attribute__((weak, alias("__stop"))) void hardfault();
__attribute__((weak, alias("__stop"))) void MEMMANAGE_handler();
__attribute__((weak, alias("__stop"))) void BUSFAULT_handler();
__attribute__((weak, alias("__stop"))) void USAGEFAULT_handler();
__attribute__((weak, alias("__stop"))) void SVCALL_handler();
__attribute__((weak, alias("__stop"))) void DEBUGMONITOR_handler();
__attribute__((weak, alias("__stop"))) void PENDSV_handler();
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
    SVCALL_handler,
    DEBUGMONITOR_handler,
    DUMMY_handler,
    PENDSV_handler,
    systick,
};
