/*
 * mfrc522.h
 *
 *  Created on: Jul 23, 2022
 *      Author: lth
 */

#ifndef MFRC522_H_
#define MFRC522_H_

#ifdef DEBUG
#define MFRC_DBG(...) printf(__VA_ARGS__);\
                     printf("\n");
#else
#define MFRC_DBG(...) ;
#endif

typedef struct {
        SPI_HandleTypeDef *spiHandle;
        GPIO_TypeDef *cs_port;
        uint16_t cs_pin;
} MFRC522_HandleTypeDef;

typedef enum {
    MFRC522_Ok,     // 0
    MFRC522_Err,    // 1
    MFRC522_Timeout // 2
} MFRC522_result_t;

MFRC522_result_t mfrc522_init(MFRC522_HandleTypeDef *mfrc522, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin);

#endif /* MFRC522_H_ */
