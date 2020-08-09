/**
@file    gpios.h
@version 0.0
@date    2020.07.15

@author  Alexander
@email   zhevak@mail.ru

@brief   Управление портами GPIO
*/


#ifndef __GPIOS_H__
#define __GPIOS_H__


#include <stm32/stm32f1xx.h>
#include "bits.h"



// Назначение битов порта PA
#define SPI1_port     (GPIOA)
#define SPI1_NCS      (_BIT(3))
#define SPI1_NSS      (_BIT(4))
#define SPI1_SCK      (_BIT(5))
#define SPI1_MISO     (_BIT(6))
#define SPI1_MOSI     (_BIT(7))

#define YLED (_BIT(1))
#define YLED_port GPIOA

#define RLED1 (_BIT(15))
#define RLED1_port GPIOA


// Порт GPIOB
#define GLED1 (_BIT(3))
#define GLED1_port GPIOB
#define YLED1 (_BIT(4))
#define YLED1_port GPIOB
#define RLED2 (_BIT(5))
#define RLED2_port GPIOB
#define GLED2 (_BIT(6))
#define GLED2_port GPIOB
#define YLED2 (_BIT(7))
#define YLED2_port GPIOB
#define RLED3 (_BIT(8))
#define RLED3_port GPIOB
#define RLED4 (_BIT(9))
#define RLED4_port GPIOB
#define BUZZER (_BIT(10))
#define BUZZER_port GPIOB


// Управление светодиодом на плате
#define YLED_OFF() (YLED_port->BSRR = YLED)
#define YLED_ON()  (YLED_port->BRR  = YLED)
#define YLED_TOGGLE() (((YLED_port->ODR & YLED) == 0) ? YLED_ON() : YLED_OFF())

// Управление светодиодом на плате
#define RLED1_ON()  (RLED1_port->BSRR = RLED1)
#define RLED1_OFF() (RLED1_port->BRR  = RLED1)
#define RLED1_TOGGLE() (((RLED1_port->ODR & RLED1) == 0) ? RLED1_ON() : RLED1_OFF())

#define GLED1_ON()  (GLED1_port->BSRR = GLED1)
#define GLED1_OFF() (GLED1_port->BRR  = GLED1)
#define GLED1_TOGGLE() (((GLED1_port->ODR & GLED1) == 0) ? GLED1_ON() : GLED1_OFF())

#define YLED1_ON()  (YLED1_port->BSRR = YLED1)
#define YLED1_OFF() (YLED1_port->BRR  = YLED1)
#define YLED1_TOGGLE() (((YLED1_port->ODR & YLED1) == 0) ? YLED1_ON() : YLED1_OFF())

#define RLED2_ON()  (RLED2_port->BSRR = RLED2)
#define RLED2_OFF() (RLED2_port->BRR  = RLED2)
#define RLED2_TOGGLE() (((RLED2_port->ODR & RLED2) == 0) ? RLED2_ON() : RLED2_OFF())

#define GLED2_ON()  (GLED2_port->BSRR = GLED2)
#define GLED2_OFF() (GLED2_port->BRR  = GLED2)
#define GLED2_TOGGLE() (((GLED2_port->ODR & GLED2) == 0) ? GLED2_ON() : GLED2_OFF())

#define YLED2_ON()  (YLED2_port->BSRR = YLED2)
#define YLED2_OFF() (YLED2_port->BRR  = YLED2)
#define YLED2_TOGGLE() (((YLED2_port->ODR & YLED2) == 0) ? YLED2_ON() : YLED2_OFF())

#define RLED3_ON()  (RLED3_port->BSRR = RLED3)
#define RLED3_OFF() (RLED3_port->BRR  = RLED3)
#define RLED3_TOGGLE() (((RLED3_port->ODR & RLED3) == 0) ? RLED3_ON() : RLED3_OFF())

#define RLED4_ON()  (RLED4_port->BSRR = RLED4)
#define RLED4_OFF() (RLED4_port->BRR  = RLED4)
#define RLED4_TOGGLE() (((RLED4_port->ODR & RLED4) == 0) ? RLED4_ON() : RLED4_OFF())


#define BUZZER_ON()  (BUZZER_port->BSRR = BUZZER)
#define BUZZER_OFF() (BUZZER_port->BRR  = BUZZER)
//#define BUZZER_TOGGLE() (((BUZZER_port->ODR & BUZZER) == 0) ? BUZZER_ON() : BUZZER_OFF())


#ifdef __cplusplus
 extern "C" {
#endif

void gpios_init(void);
void gpios_testing(void);

#ifdef __cplusplus
}
#endif


#endif  // __GPIOS_H__
