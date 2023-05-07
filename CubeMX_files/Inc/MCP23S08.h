#ifndef NUCLEO_F446RE_MCP23S08_H
#define NUCLEO_F446RE_MCP23S08_H

#include "stm32f4xx_hal.h"
#include "spi.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"

//register_address
#define MCP_IODIR		0x00
#define MCP_IPOL		0x01
#define MCP_GPINTEN		0x02
#define MCP_DEFVAL		0x03
#define MCP_INTCON		0x04
#define MCP_IOCON		0x05
#define MCP_GPPU		0x06
#define MCP_INTF		0x07
#define MCP_INTCAP		0x08
#define MCP_GPIO		0x09
#define MCP_OLAT		0x0A

//GPIO
#define GP0             0x01
#define GP1             0x02
#define GP2             0x04
#define GP3             0x08
#define GP4             0x10
#define GP5             0x20
#define GP6             0x40
#define GP7             0x80

extern SPI_HandleTypeDef hspi;

void MCP23S08_write(uint8_t device_address, uint8_t register_address, uint8_t value);
uint8_t MCP23S08_read(uint8_t device_address, uint8_t register_address);
void MCP23S08_init(uint8_t device_address, SPI_HandleTypeDef *hspi_mcp, GPIO_TypeDef* reset_port, uint16_t reset_pin, uint8_t input_pins);
void MCP23S08_set_pullup_resistor(uint8_t device_address, uint8_t pullup_pins);
void MCP23S08_GPIO_WritePin(uint8_t device_address, uint8_t GPIO_pin, uint8_t GPIO_pin_state);
void MCP23S08_GPIO_TogglePin(uint8_t device_address, uint8_t GPIO_pin);
int MCP23S08_GPIO_ReadPin(uint8_t device_address, uint8_t GPIO_pin);

#endif //NUCLEO_F446RE_MCP23S08_H
