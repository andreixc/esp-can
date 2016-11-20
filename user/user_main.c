#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_config.h"
#include "eagle_soc.h"
#include "button.h"
#include "uart.h"

#define user_procTaskPrio        0
#define user_procTaskQueueLen    1
os_event_t    user_procTaskQueue[user_procTaskQueueLen];
static void user_procTask(os_event_t *events);

static volatile os_timer_t some_timer;

void user_rf_pre_init(){}

void some_timerfunc(void *arg)
{
    //Do blinky stuff

	led_toggle();
	//os_printf("GPIO[0] %d\r\n",button_read());-
	os_printf("TICK\r\n");
}

void press(void)
{
	os_printf("P\r\n");
}

void release(void)
{
	os_printf("R\r\n");
}

//Do nothing function
static void ICACHE_FLASH_ATTR
user_procTask(os_event_t *events)
{
    os_delay_us(10);
}

//Init function 
void ICACHE_FLASH_ATTR
user_init()
{

	// Initialize the GPIO subsystem.
    gpio_init();

    uart_init(BIT_RATE_115200,BIT_RATE_115200);
    button_init();
    led_init();

    //Disarm timer
    os_timer_disarm(&some_timer);

    //Setup timer
    os_timer_setfn(&some_timer, (os_timer_func_t *)some_timerfunc, NULL);

    //Arm the timer
    //&some_timer is the pointer
    //1000 is the fire time in ms
    //0 for once and 1 for repeating
    os_timer_arm(&some_timer, 1000, 1);
    
    // btn callbacks
    button_register_events(press,release);

    //Start os task
    system_os_task(user_procTask, user_procTaskPrio,user_procTaskQueue, user_procTaskQueueLen);
}
