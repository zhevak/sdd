/**
@file    gpios.c
@version 0.0
@date    2020.07.15

@author  Alexander
@email   zhevak@mail.ru

@brief   Управление портами GPIO
*/


#include <stm32/stm32f1xx.h>

#include "systick.h"
#include "gpios.h"



/**
@brief  Производит настройку портов ввода/вывода общего назначения
@param  нет
@retval нет
*/
void gpios_init(void)
{
  // Включаем тактирование портов GPIOA и GPIOB6
  
  RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
  
  // Отключим JTAG on PB3, PB4 и PA15
  AFIO->MAPR &= ~(AFIO_MAPR_SWJ_CFG_2 | AFIO_MAPR_SWJ_CFG_0);
  AFIO->MAPR |=  (AFIO_MAPR_SWJ_CFG_1);  

  // RCC->APB2ENR &= ~RCC_APB2ENR_AFIOEN;
  
  // Выходы для светодиодов (MODE = 0b10, CNF = 0b00)
  GPIOA->CRL = (GPIOA->CRL & 0xFFFF0F0F) | 0x00002020;  // SDCARD, YLED
  GPIOA->CRH = (GPIOA->CRH & 0x0FFFFFFF) | 0x20000000;  // RLED1
  GPIOB->CRL = (GPIOB->CRL & 0x00000FFF) | 0x22222000;  // GLED1, YLED1, RLED2, GLED2, YLED2
  GPIOB->CRH = (GPIOB->CRH & 0xFFFFF000) | 0x00000222;  // RLED3, RLED4, BUZZER

  // Обнулим выходы за исключением SDCARD_NCS и YLED, они должны остаться в "1".
  // Это нужно сделать, так как после отключения JTAG, вывходы PB4 и PA15 остаются в "1"
  GPIOA->ODR = SPI1_NCS | YLED;
  GPIOB->ODR = 0x0000;
}



/**
@brief  Тестовое зажигание светодиодов
@param  нет
@retval нет
*/
void gpios_testing(void)
{
  YLED_ON();
  delay_ms(100);
  YLED_OFF();

  RLED1_ON();
  delay_ms(100);
  RLED1_OFF();

  GLED1_ON();
  delay_ms(100);
  GLED1_OFF();

  YLED1_ON();
  delay_ms(100);
  YLED1_OFF();
  
  RLED2_ON();
  delay_ms(100);
  RLED2_OFF();
  
  GLED2_ON();
  delay_ms(100);
  GLED2_OFF();
  
  YLED2_ON();
  delay_ms(100);
  YLED2_OFF();
  
  RLED3_ON();
  delay_ms(100);
  RLED3_OFF();

  RLED4_ON();
  delay_ms(100);
  RLED4_OFF();

  BUZZER_ON();
  delay_ms(100);
  BUZZER_OFF();
}
