#include "MCP23S08.h"

SPI_HandleTypeDef hspi;

void MCP23S08_write(uint8_t device_address, uint8_t register_address, uint8_t value) {
    uint8_t spi_data[3] = {device_address, register_address, value};
    HAL_GPIO_WritePin(SPI2_CS1_GPIO_Port, SPI2_CS1_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi, spi_data, sizeof(spi_data), HAL_MAX_DELAY);
    HAL_GPIO_WritePin(SPI2_CS1_GPIO_Port, SPI2_CS1_Pin, GPIO_PIN_SET);
}

uint8_t MCP23S08_read(uint8_t device_address, uint8_t register_address) {
    uint8_t spi_data[3] = {device_address, register_address, 0xFF};

    char m[100];
    sprintf(m, "b0: %d\r\nb1: %d\r\nb2: %d\r\n\n", spi_data[0], spi_data[1], spi_data[2]);
    HAL_UART_Transmit(&huart2, (uint8_t *)m, strlen(m), HAL_MAX_DELAY);

    HAL_GPIO_WritePin(SPI2_CS1_GPIO_Port, SPI2_CS1_Pin, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(&hspi, spi_data, spi_data, sizeof(spi_data), HAL_MAX_DELAY);
    HAL_GPIO_WritePin(SPI2_CS1_GPIO_Port, SPI2_CS1_Pin, GPIO_PIN_SET);

    sprintf(m, "a0: %d\r\na1: %d\r\na2: %d\r\n\n", spi_data[0], spi_data[1], spi_data[2]);
    HAL_UART_Transmit(&huart2, (uint8_t *)m, strlen(m), HAL_MAX_DELAY);

    return spi_data[2];
}

void MCP23S08_init(uint8_t device_address, SPI_HandleTypeDef *hspi_mcp, GPIO_TypeDef* reset_port, uint16_t reset_pin, uint8_t input_pins) {
    //reset MCP28S03
    HAL_GPIO_WritePin(reset_port, reset_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(reset_port, reset_pin, GPIO_PIN_SET);
    //set spi
    hspi = *hspi_mcp;
    //write configuration
    MCP23S08_write(device_address, MCP_IODIR, input_pins);
}

void MCP23S08_set_pullup_resistor(uint8_t device_address, uint8_t pullup_pins) {
    MCP23S08_write(device_address, MCP_GPPU, pullup_pins);
}

void MCP23S08_GPIO_WritePin(uint8_t device_address, uint8_t GPIO_pin, uint8_t GPIO_pin_state) {
    //read outputs states
    uint8_t gpio = MCP23S08_read(device_address | 0x01, MCP_GPIO);
    if (GPIO_pin_state == GPIO_PIN_SET) {
        gpio |= GPIO_pin;
        MCP23S08_write(device_address, MCP_OLAT, gpio);
    }
    else {
        gpio &= (~GPIO_pin);
        MCP23S08_write(device_address, MCP_OLAT, gpio);
    }
}

void MCP23S08_GPIO_TogglePin(uint8_t device_address, uint8_t GPIO_pin) {
    //read outputs states
    uint8_t gpio = MCP23S08_read(device_address | 0x01, MCP_GPIO);
    gpio ^= GPIO_pin;
    MCP23S08_write(device_address, MCP_OLAT, gpio);
}

int MCP23S08_GPIO_ReadPin(uint8_t device_address, uint8_t GPIO_pin) {
    //read outputs states
    uint8_t gpio = MCP23S08_read(device_address | 0x01, MCP_GPIO);
    uint8_t gpio_direction = MCP23S08_read(device_address | 0x01, MCP_IODIR);
    //pin direction - output
    if ((gpio_direction & GPIO_pin) == 0x00) {
        return -1;
    }
    //pin direction - input
    else {
        if ((gpio & GPIO_pin) == 0x00) {
            return 0;
        } else {
            return 1;
        }
    }
}