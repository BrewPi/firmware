
#include "application.h"
#include "freertos4core.h"


#if !defined(SYSTEM_VERSION_046)
#error "FreeRTOS4Core requires System firmware version 0.4.6 or newer."
#endif

#if !defined(PLATFORM_ID) || PLATFORM_ID>2
#error "FreeRTOS4Core is not suitable for this platform. It is only suitable for the Core"
#endif



/**
 * The mutex to ensure only one thread manipulates the heap at a given time.
 */
xSemaphoreHandle malloc_mutex = 0;

static void init_malloc_mutex(void)
{
    malloc_mutex = xSemaphoreCreateRecursiveMutex();
}

extern "C" void __malloc_lock(void* ptr)
{
    if (malloc_mutex)
        while (!xSemaphoreTakeRecursive(malloc_mutex, 0xFFFFFFFF)) {}
}

extern "C" void __malloc_unlock(void* ptr)
{
    if (malloc_mutex)
        xSemaphoreGiveRecursive(malloc_mutex);
}

void rtos_start(TaskHandle_t handle)
{
    app_setup_and_loop();
}

static TaskHandle_t  app_thread_handle;
#define APPLICATION_STACK_SIZE 1024*2


extern "C" void HAL_Hook_Main()
{
    init_malloc_mutex();
    xTaskCreate( rtos_start, "app_thread", APPLICATION_STACK_SIZE/sizeof( portSTACK_TYPE ), NULL, 2, &app_thread_handle);

    vTaskStartScheduler();
    while(1);
}

extern "C" void vApplicationTickHook()
{

}


