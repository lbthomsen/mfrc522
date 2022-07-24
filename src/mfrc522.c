/*
 * Minimal mrf522 library
 *
 * Copyright (c) 2022 Lars Boegild Thomsen <lbthomsen@gmail.com>
 *
 */

#include "main.h"
#include "string.h"
#include "mfrc522.h"

#ifdef DEBUG
#include "stdio.h"
#endif

/**
 * @brief	Switch CS (Chip Select) on - active low
 * @param	MFRC522 handle
 * @retval	none
 */
void mfrc522_cs_on(MFRC522_HandleTypeDef *mfrc522) {
    HAL_GPIO_WritePin(mfrc522->cs_port, mfrc522->cs_pin, GPIO_PIN_RESET);
}

/**
 * @brief	Switch CS (Chip Select) off - active low
 * @param	MFRC522 handle
 * @retval	none
 */
void mfrc522_cs_off(MFRC522_HandleTypeDef *mfrc522) {
    HAL_GPIO_WritePin(mfrc522->cs_port, mfrc522->cs_pin, GPIO_PIN_SET);
}

/**
 * @brief	Write characters to specific address
 * @param	MFRC522_Handle
 * @param   Address
 * @param	Value
 * @return  MFRC522_result_t
 */
MFRC522_result_t mfrc522_write(MFRC522_HandleTypeDef *mfrc522, unsigned char addr, unsigned char *val, uint8_t len) {

    MFRC522_result_t result = MFRC522_Ok;
    unsigned char addr_bits = (((addr << 1) & 0x7E));

    mfrc522_cs_on(mfrc522);

    if (HAL_SPI_Transmit(mfrc522->spiHandle, &addr_bits, 1, HAL_MAX_DELAY) == HAL_OK) {
        if (HAL_SPI_Transmit(mfrc522->spiHandle, val, len, HAL_MAX_DELAY) != HAL_OK) {
            result = MFRC522_Err;
        }
    }

    mfrc522_cs_off(mfrc522);

    return result;
}

/**
 * @brief   Read characters from specific address
 * @param   MFRC522_Handle
 * @param   Address
 * @param   Value
 * @return  MFRC522_result_t
 */
MFRC522_result_t mfrc522_read(MFRC522_HandleTypeDef *mfrc522, unsigned char addr, unsigned char *val, uint8_t len) {

    MFRC522_result_t result = MFRC522_Ok;
    unsigned char addr_bits = (((addr << 1) & 0x7E) | 0x80);

    mfrc522_cs_on(mfrc522);

    if (HAL_SPI_Transmit(mfrc522->spiHandle, &addr_bits, 1, HAL_MAX_DELAY) == HAL_OK) {
        if (HAL_SPI_Receive(mfrc522->spiHandle, val, len, HAL_MAX_DELAY) != HAL_OK) {
            result = MFRC522_Err;
        }
    }

    mfrc522_cs_off(mfrc522);

    return result;
}

/**
 * @brief   Set MFRC522 register bit
 * @param   MFRC522_Handle
 * @param   Register
 * @param   Mask
 * @return  MFRC522_result_t
 */
MFRC522_result_t mfrc522_set_bits(MFRC522_HandleTypeDef *mfrc522, unsigned char reg, unsigned char mask) {

    MFRC522_result_t result = MFRC522_Ok;

    unsigned char tmp;

    if (mfrc522_read(mfrc522, reg, &tmp, 1) == MFRC522_Ok) {
        tmp |= mask;
        if (mfrc522_write(mfrc522,reg, &tmp, 1) != MFRC522_Ok) {
            result = MFRC522_Err;
        }
    } else {
        result = MFRC522_Err;
    }

    return result;
}

/**
 * @brief   Clear MFRC522 register bit
 * @param   MFRC522_Handle
 * @param   Register
 * @param   Mask
 * @return  MFRC522_result_t
 */
MFRC522_result_t mfrc522_clear_bits(MFRC522_HandleTypeDef *mfrc522, unsigned char reg, unsigned char mask) {

    MFRC522_result_t result = MFRC522_Ok;

    unsigned char tmp;

    if (mfrc522_read(mfrc522, reg, &tmp, 1) == MFRC522_Ok) {
        tmp &= (~mask);
        if (mfrc522_write(mfrc522,reg, &tmp, 1) != MFRC522_Ok) {
            result = MFRC522_Err;
        }
    } else {
        result = MFRC522_Err;
    }

    return result;
}

/**
 * @brief   Switch on antenna
 * @param   MFRC522_Handle
 * @return  MFRC522_result_t
 */
MFRC522_result_t mfrc522_antenna_on(MFRC522_HandleTypeDef *mfrc522) {

    return mfrc522_set_bits(mfrc522, TxControlReg, 0x03);

}

/**
 * @brief   Switch off antenna
 * @param   MFRC522_Handle
 * @return  MFRC522_result_t
 */
MFRC522_result_t mfrc522_antenna_off(MFRC522_HandleTypeDef *mfrc522) {

    return mfrc522_clear_bits(mfrc522, TxControlReg, 0x03);

}

MFRC522_result_t mfrc522_init(MFRC522_HandleTypeDef *mfrc522, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin) {

    MFRC522_result_t result = MFRC522_Ok;

    MFRC_DBG("Initializing MFRC522");

    mfrc522->spiHandle = hspi;
    mfrc522->cs_port = cs_port;
    mfrc522->cs_pin = cs_pin;

    return result;

}
