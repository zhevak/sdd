/**
@file    syscalls.c
@version 0.0
@date    2020.07.15

@author  Alexander
@email   zhevak@mail.ru

@brief   Системные вызовы
*/


#include <stm32/stm32f1xx.h>

//TODO нужно было для олтладки
#include "gpios.h"


#include "usart1.h"

//#include "system.h"



/**
 * @brief  Функция вызыввается до входа в main()
 * @param  нет
 * @retval нет
*/
void SystemInit(void)
{
  // Этот блок инициализации нужен только для работы от чистого HSE, без PLL
  //RCC->CFGR &= ~RCC_CFGR_SW;
  //RCC->CFGR |= RCC_CFGR_SW_HSE; // Переключаемся на HSE
  
  RCC->CFGR &= ~RCC_CFGR_SW;    // Предочистка

  // Переключаемся на внешний кварцевый резоатор 8 МГц
  RCC->CR |= RCC_CR_HSEON; //Включаем генератор
  while (!(RCC->CR & RCC_CR_HSERDY))
    ; // Ожидаем стабилизации колебаний

  RCC->CFGR2 &= ~RCC_CFGR2_PREDIV1;      // Предочистка всех битов
  RCC->CFGR2 |=  RCC_CFGR2_PREDIV1_DIV2; // Устанавливаем делитель на 2 (8 МГц / 2 = 4 МГц)
  
  RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL); // Предочистка
  RCC->CFGR |=  (RCC_CFGR_PLLSRC | // Будем тактировать PLL от HSE через предделитель PREDIV1
                 RCC_CFGR_PLLMULL6);       // Умножим на 6 (4 МГц * 6 = 24 МГц)
  
  RCC->CR   |= RCC_CR_PLLON;       // Запускаем работу PLL  
  while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    ; // Ждем когда PLL выйдет на рабочий режим

  RCC->CFGR |= RCC_CFGR_SW_PLL; // Переключаемся на PLL
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
    ; // Ожидаем переключения на PLL

//  RCC->CR &= ~RCC_CR_HSION; // Выключаем генератор HSI, чтобы не жрал лишнюю энергию

  // TODO Включить систему защиты от нестабильной работы и отказов генератора HSE
  // TODO Прописать соответствующий обработчик немаскируемого прерывания
}

