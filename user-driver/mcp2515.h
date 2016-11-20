/*
 * mcp2515.h
 *
 *  Created on: 13 Nov 2016
 *      Author: eraser
 */

#include <c_types.h>

#ifndef USER_DRIVER_MCP2515_H_
#define USER_DRIVER_MCP2515_H_

/************** SPI Layer **************/

uint8_t spi_init(void);
uint8_t spi_rx_tx(uint8_t);

/************** MCP2515 Layer **************/

#define HSCAN_CS	(0)
#define MSCAN_CS	(1)
#define SELECT		(1)
#define DESELECT	(0)

uint8_t mcp2515_init(void);
void mcp2515_select(uint8_t,uint8_t);

#endif
