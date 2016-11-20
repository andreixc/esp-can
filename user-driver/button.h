/*
 * button.h
 *
 *  Created on: 12 Nov 2016
 *      Author: eraser
 */

#include <c_types.h>

#ifndef BUTTON_BUTTON_H_
#define BUTTON_BUTTON_H_

void button_init(void);
uint8_t button_read(void);
void button_register_events(void (*btn_press)(void),void (*btn_release)(void));

void led_init(void);
void led_state(uint8_t);
uint8_t led_read(void);

#endif /* BUTTON_BUTTON_H_ */
