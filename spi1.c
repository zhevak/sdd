/**
@file    spi1.c
@version 0.0
@date    2020.07.17

@author  Alexander
@email   zhevak@mail.ru

@brief   вставьте сюда краткое описание файла
*/



#include <stdbool.h>
#include <stddef.h>

#include <stm32/stm32f1xx.h>

#include "gpios.h"

#include "spi1.h"



// Настараиваем SPI1
//FIXME Тактовая частота SCK = F_PCLK2 / 256 = 93.75 кГц
// Тактовая частота SCK: 1.5 МГц (Делитель BR = 0b011: PCLK / 16 = 24 / 16 = 1.5)
// Старшим битом вперёд
// Режим работы SPI: 3 (CPOL = 1, CPHA = 1)
// Мастер: MSTR
void spi1_init(void)
{
  // Включаем тактирование SPI1, портов PA, а также периферийных функций
  RCC->APB2ENR |= (RCC_APB2ENR_SPI1EN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN);

  // Настраиваем выводы порта PA под работу SPI

  // SPI_MOSI: GPIOA.7  Out, LF, AFPP:  MODE = 0b10, CNF = 0b10
  // SPI_MISO: GPIOA.6  In,  Z:         MODE = 0b00, CNF = 0b01
  // SPI_SCK:  GPIOA.5  Out, LF, AFPP:  MODE = 0b10, CNF = 0b10
  // SPI_NSS:  GPIOA.4  Out, LF, PP:    MODE = 0b10, CNF = 0b00
  // SPI_NCS:  GPIOA.3  Out, LF, PP:    MODE = 0b10, CNF = 0b00

  GPIOA->CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7 |
                  GPIO_CRL_MODE6 | GPIO_CRL_CNF6 |
                  GPIO_CRL_MODE5 | GPIO_CRL_CNF5 |
                  GPIO_CRL_MODE4 | GPIO_CRL_CNF4 |
                  GPIO_CRL_MODE3 | GPIO_CRL_CNF3);

  GPIOA->CRL |=  (GPIO_CRL_MODE7_1 | GPIO_CRL_CNF7_1 |
                                     GPIO_CRL_CNF6_0 |
                  GPIO_CRL_MODE5_1 | GPIO_CRL_CNF5_1 |
                  GPIO_CRL_MODE4_1 |
                  GPIO_CRL_MODE3_1);

  GPIOA->BSRR = SPI1_NSS;  // Установить NSS в единицу и больше никогда не трогать!
  SPI1_NCS_1();            // Установить NCS в неактивное состояние
  SPI1->CR1  = (SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0 | SPI_CR1_MSTR); // | SPI_CR1_CPOL | SPI_CR1_CPHA);
  SPI1->CR1 |= SPI_CR1_SPE; // И наконец включаем SPI1
}


uint8_t spi1_rw(uint8_t data)
{
  uint8_t byte;
  
  while (SPI1->SR & SPI_SR_BSY)
    ;  // Ждём освобождения SPI1
  
  byte = SPI1->DR;  // Этой командой снимаем флаг RXNE
  
  SPI1->DR = data;  // Передаём байт
  while (!(SPI1->SR & SPI_SR_RXNE))
    ;  // Ждем окончания чтения байта

  byte = SPI1->DR;

  return byte;
}



uint8_t spi1_readByte(void)
{
  uint8_t byte;
  
  while (SPI1->SR & SPI_SR_BSY)
    ;  // Ждём освобождения SPI1
  
  byte = SPI1->DR;  // Этой командой снимаем флаг RXNE
  
  SPI1->DR = 0xFF;  // Символ-заполнитель для SD-карт
  while (!(SPI1->SR & SPI_SR_RXNE))
    ;  // Ждем окончания чтения байта

  byte = SPI1->DR;

  return byte;
}



void spi1_writeByte(uint8_t byte)
{
  // Атрибут позволяет при компиляции избежать вывода сообщения, что переменная не используется
  uint8_t del __attribute__((unused));

  while (SPI1->SR & SPI_SR_BSY)
    ; // Ждём освобождения SPI1
  
  del = SPI1->DR;  // Чтение регистра DR снимает флаг RXNE

  SPI1->DR = byte;

  while (!(SPI1->SR & SPI_SR_RXNE))
    ;  // Ждём окончания передачи байта
}



void spi1_read(uint8_t *buf, int len)
{
  for (int i = 0; i < len; i++)
  {
    SPI1->DR = 0xFF;  // Символ-заполнитель для SD-карт
    while (!(SPI1->SR & SPI_SR_RXNE))
      ;  // Ждем окончания чтения байта
    buf[i] = SPI1->DR;
  }
}



void spi1_write(const uint8_t *buf, int len)
{
  // Атрибут позволяет при компиляции избежать вывода сообщения, что переменная не используется
  uint8_t del __attribute__((unused));

  for (int i = 0; i < len; i++)
  {
    while (SPI1->SR & SPI_SR_BSY)
      ; // Ждём освобождения SPI1
  
    del = SPI1->DR;  // Чтение регистра DR снимает флаг RXNE
  
    SPI1->DR = buf[i];
    while (!(SPI1->SR & SPI_SR_RXNE))
      ;  // Ждём окончания передачи байта
  }
}


