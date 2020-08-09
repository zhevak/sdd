// usart1.c

/**
 * Последовательный порт для работы с компом
*/ 


#include <stm32/stm32f1xx.h>
#include <stdbool.h>

#include "usart1.h"
#include "gpios.h"


#define RXBUFSIZE (10)
#define TXBUFSIZE (10)


uint8_t  rxBuf[RXBUFSIZE];      // Кольцевой буфер
volatile uint8_t rxIn;          // Индекс места для входного символа
volatile uint8_t rxOut;         // Индекс места для выходного символа

uint8_t  txBuf[TXBUFSIZE];      // Буфер передатчика
volatile uint8_t txIn;          // Индекс места для входного символа
volatile uint8_t txOut;         // Индекс места для выходного символа


/**
 * @brief  Обработчик прерывания от USART1
 * @param  нет
 * @retval нет
*/
void USART1_IRQHandler(void)
{
  uint8_t data;
  
  // Приёмник --------------------------------------------------------
  if (USART1->CR1 & USART_CR1_RXNEIE)
  { // Прерывание от приёмника
    if (USART1->SR & USART_SR_ORE)
    { // Переполнение по приёму
      USART1->SR &= ~(USART_SR_ORE | USART_SR_RXNE); // Снимаем флаги
      USART1->CR1 &= ~USART_CR1_RE;  // Останавливаем работу приёмника
    }
    else if (USART1->SR & USART_SR_RXNE)
    { // Принят байт
      data = USART1->DR;
      
      uint8_t next = rxIn + 1;
      
      if (next == RXBUFSIZE)
        next = 0;
        
      if (next == rxOut)
      { // Кольцевой Буфер заполнени полностью. Принятый байт откидываем
        ;
      }
      else
      { // В кольцевом буфере есть место для сохранения принятого байта
        rxBuf[rxIn] = data;
        rxIn = next;
      }
      
      /*
      if (++rxIn >= RXBUFSIZE)      
      { // Закольцовка буфера
        rxIn = 0;
      }
      if (rxIn == rxOut)
      { // Переполнение входного кольцевого буфера
        USART1->CR1 &= ~USART_CR1_RE;  // Останавливаем работу приёмника
      }
      */
    }
  }

  // Передатчик -------------------------------------------------------
  if ((USART1->SR & USART_SR_TXE) && (USART1->CR1 & USART_CR1_TXEIE))
  { // Входной буфер передатчика пустой, можно засылать очередной байт
    if (txOut != txIn)
    { // В кольцевом буфере есть байты для отправки
      USART1->DR = txBuf[txOut];
    
      txOut++;
      if (txOut >= TXBUFSIZE)
        txOut = 0;  // Закольцовка

      if (txOut == txIn)
      { // Это был последний байт сообщения
        // Теперь меняем источник перывания
        USART1->CR1 &= ~USART_CR1_TXEIE; // Сменим тип прерывания
        USART1->CR1 |=  USART_CR1_TCIE;
      }
    }
    else
    { // Кольцевой буфер не содержит байтов для отправки
      USART1->CR1 &= ~(USART_CR1_TCIE);  // Выключаем прывания от передатчика
    }
  }
  if ((USART1->SR & USART_SR_TC) && (USART1->CR1 & USART_CR1_TCIE))
  { // Передача закончена полностью
    USART1->CR1 &= ~(USART_CR1_TCIE);  // Выключаем прывания от передатчика
    
    // TODO Погасить TX_LED
    GLED2_OFF();
  }
}



/**
 * @brief  Инициализация USART1
 *         115200 baud 8N1
 * @param  нет
 * @retval нет
*/
void usart1_init(void)
{
  //Включаем тактирование порта А и тактирование альтернативной ф-ции
  RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN);

  // Вывод PA9 (TX)
  GPIOA->CRH |=  (GPIO_CRH_MODE9_1 | GPIO_CRH_CNF9_1);	// режим выхода, 2МГц
  GPIOA->CRH &= ~(GPIO_CRH_MODE9_0 | GPIO_CRH_CNF9_0);	// двухтактный выход альтернативной функции
  
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;	 // включаем тактирование USART1
  
  USART1->BRR = (24000000L / 115200 + 1);

  NVIC_EnableIRQ(USART1_IRQn);  // Разрешаем прерывания

  USART1->CR1 |= USART_CR1_UE;  // Включаем USART1

  rxOut = rxIn = 0; // Начинаем всё с начала
  USART1->SR &= ~(USART_SR_ORE | USART_SR_RXNE); // Снимаем флаги  
  USART1->CR1 |= (USART_CR1_RXNEIE | USART_CR1_RE);  // Включаем приёмник и разрешаем прерывания от приёмника

  txOut = txIn = 0; // Начинаем всё с начала
  USART1->CR1 |= (USART_CR1_TE); // Включаем передатчик
}



bool usart1_readByte(uint8_t *byte)
{
  bool result = false;
  
  if (rxOut != rxIn)
  { // В кольцевом буфере есть принятые, но извлечённые байты
    *byte = rxBuf[rxOut];
    
    rxOut++;
    if (rxOut == RXBUFSIZE)
      rxOut = 0;

    result = true;
  }
  else
  { // В кольцевом буфере нет принятых байтов
    ;
  }
  

  /*  
  USART1->CR1 &= ~USART_CR1_RXNEIE; // Временно запрещаем прерывания от приёмника
  {
    if (rxIn != rxOut)
    {
      *byte = rxBuf[rxOut];
      
      if (++rxOut >= RXBUFSIZE)
        rxOut = 0;
  
      result = true;
    }
  }
  USART1->CR1 |= USART_CR1_RXNEIE; // Разрешаем прерывания от приёмника

  // Включаем приёмник.
  // Он мог быть остановлен по переполнению входного кольцевого буфера.
  USART1->CR1 |= USART_CR1_RE;
  */
  return result;
}



/**
 * @brief Посилает один байт в комп
 * @param byte байт для пересылки в комп
 * @retval
*/
void usart1_writeByte(uint8_t byte)
{
  uint8_t next = txIn + 1;

  // TODO Погасить TX_LED
  GLED2_OFF();

  if (next >= TXBUFSIZE)
    next = 0; // Закольцовка буфера
    
  while (next == txOut)
    ; // Ждём момента, когда освободится место в кольцевом буфере передатчика для символа

  txBuf[txIn] = byte; // Записываем в кольцевой буфер передатчика
  txIn = next;

  // Разрешаем прерывание по пустому буферу передатчика.
  // Если прерывание было разрешено, то повторное разрешение не повлияет на работу передатчика.
  USART1->CR1 |= (USART_CR1_TXEIE);
}


/**
 * @brief Послать пакет в комп
 * @param pPack пакет для пересылки в комп
 * @param len длина пакет
 * @retval
*/
/*
int usart1_send(const uint8_t *pPack, int len)
{
  int i;
  
  if (len > TXBUFSIZE)
    return EOVER;  // Длина сообщения превышает размер буфера передатчика

  while (fTxBusy == BUSY)
    ; // Ждём освобождения передатчика

  fTxBusy = BUSY;  // Устанавливаем флаг занятости передатчика

  // Копируем сообщение в буфер передатчика
  for (i = 0; i < len; i++)
    txBuf[i] = pPack[i];
    
  txIndex = 0;  // Устанавливаем индекс текущего байта для передачи
  txLen = len;  // Длина сообщения
  

// TODO Зажечь TX_LED
GLED2_ON();
  
  return OK;
}



bool usart1_isTxReady(void)
{
  if (fTxBusy == READY)
    return true;   // Передатчик готов к передаче следующего сообщения
  else
    return false;  // Передатчик занят
}
*/
