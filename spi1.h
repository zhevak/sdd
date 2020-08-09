/**
@file    spi1.h
@version 0.0
@date    2020.07.17

@author  Alexander
@email   zhevak@mail.ru

@brief   вставьте сюда краткое описание файла
*/


#ifndef __SPI1_H__
#define __SPI1_H__


#include "gpios.h"


// Управление сигналом nCS
#define SPI1_NCS_0()  {SPI1_port->BRR  = SPI1_NCS;}
#define SPI1_NCS_1()  {SPI1_port->BSRR = SPI1_NCS;}


#ifdef __cplusplus
 extern "C" {
#endif


void    spi1_init(void);
uint8_t spi1_readByte(void);
void    spi1_writeByte(uint8_t byte);
void    spi1_read(uint8_t *buf, int len);
void    spi1_write(const uint8_t *buf, int len);

uint8_t spi1_rw(uint8_t);

#ifdef __cplusplus
}
#endif


#endif  // __SPI1_H__
