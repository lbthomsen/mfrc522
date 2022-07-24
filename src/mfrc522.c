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


MFRC522_result_t mfrc522_init(MFRC522_HandleTypeDef *mfrc522, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin) {

    MFRC522_result_t result = MFRC522_Ok;

    MFRC_DBG("Initializing MFRC522");

    mfrc522->spiHandle = hspi;
    mfrc522->cs_port = cs_port;
    mfrc522->cs_pin = cs_pin;

    return result;

}
