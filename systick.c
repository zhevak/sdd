// systick.c
/**
 * @brief  Системный таймер
*/

#include <stm32/stm32f1xx.h>

#include "systick.h"

/**
 * @brief  Счетчик для функции delay_ms()
*/
volatile static int timer;

/**
 * @brief  Флаг односекундного интервала
*/
volatile int f1s;


/**
 * @brief  Инициализация Системного Таймера
 * @param  нет
 * @retval нет
*/
void systick_init(void)
{
  SysTick->LOAD = 24000000L / 1000 - 1;  // Будем тикать каждую миллисекунду
  // SysTick->LOAD = SysTick->CALIB;     // & SysTick_CALIB_TENMS_Msk;
  SysTick->VAL  = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
  
  f1s = 0;
}


/**
 * @brief  Обработчик прерывания Системного Таймера
 *   Прерывание возникает каждую миллисекунду (1000 раз в секунду)
 * @param  нет
 * @retval нет
*/
void SysTick_Handler(void)
{
  static int counter1s;  // Счетчик для формирования односекундных интервалов
  
  // Таймер функции delay_ms()
  if (timer)
    --timer;
    
  if (++counter1s == 1000)
  {
    counter1s = 0;
    f1s = 1;           // Установить флаг односекундногоинтервала
  }
}


/**
@function void delay_ms(uint31_t dt)
@brief  производит задержку на время dt
@param  время в миллисекундах
@retval нет       
*/
void delay_ms(uint32_t dt)
{
  //TODO Проверять, что время dt не равно 0 или 1

  if (timer == 0)
  {
    timer = dt;

    while (timer)
      ;  // Ждать
  }
  // TODO -- что будем делать, если таймер не своободен?
}
