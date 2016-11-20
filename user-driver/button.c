#include "../user-driver/button.h"

#include "ets_sys.h"
#include "osapi.h"
#include "osapi.h"
#include "user_interface.h"
#include "mem.h"
#include "os_type.h"
#include "gpio.h"
#include "uart.h"
#include "uart_register.h"

/* Button implementation */
#define	GPIO(x)	(x)
#define GPIO_0	(0)

static void (*button_pressed)(void) = NULL;
static void (*button_release)(void) = NULL;

static void gpio_intr_handler(void * arg)
{
	uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
	uint32 state = GPIO_INPUT_GET(GPIO_0);
	if(gpio_status & BIT(0))
	{
		gpio_pin_intr_state_set(GPIO_ID_PIN(0),GPIO_PIN_INTR_DISABLE);
		if(state == 0)
		{
			// call button pressed
			if(button_pressed) button_pressed();

		}
		else
		{
			// call button released
			if(button_release) button_release();
		}
		GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status & BIT(0));
		gpio_pin_intr_state_set(GPIO_ID_PIN(0),GPIO_PIN_INTR_ANYEDGE);
	}
}

void button_init(void)
{

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);
	GPIO_DIS_OUTPUT(GPIO_0);
	 /* button irq */
	ETS_GPIO_INTR_DISABLE();
	ETS_GPIO_INTR_ATTACH(gpio_intr_handler,NULL);
	gpio_register_set(GPIO_PIN_ADDR(GPIO_0),
			 GPIO_PIN_INT_TYPE_SET(GPIO_PIN_INTR_DISABLE)  |
			 GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_DISABLE) |
			 GPIO_PIN_SOURCE_SET(GPIO_AS_PIN_SOURCE)
	);
	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(0));
	gpio_pin_intr_state_set(GPIO_ID_PIN(0), GPIO_PIN_INTR_ANYEDGE);
	ETS_GPIO_INTR_ENABLE();
}

uint8_t button_read(void)
{
	return GPIO_INPUT_GET(GPIO_0);
}

void button_register_events(void (*btn_press)(void),void (*btn_release)(void))
{
	button_pressed = btn_press;
	button_release = btn_release;
}

/* Led implementation */
#define	GPIO_2	(2)
static uint8_t state;
void led_init(void)
{
    //Set GPIO2 to output mode
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
    //Set GPIO2 low
    gpio_output_set(0, BIT2, BIT2, 0);
    state = 0;
}

void led_state(uint8_t st)
{
	if(st == 0)
	{
		gpio_output_set(0, BIT2, BIT2, 0);
		state = 0;
	}
	else
	{
		gpio_output_set(BIT2, 0, BIT2, 0);
		state = 1;
	}
}

void led_toggle(void)
{
	if(led_read())
	{
		led_state(0);
	}
	else
	{
		led_state(1);
	}
}

uint8_t led_read(void)
{
	uint8_t v = GPIO_REG_READ(GPIO_OUT_ADDRESS) & BIT2;
	return (!!v);
}
