// main.c


#include <stm32/stm32f1xx.h>
#include <stdbool.h>
#include <string.h>

#include <stdio.h>

#include "gpios.h"
#include "usart1.h"
#include "spi1.h"
#include "systick.h"



void cmd_crack(uint8_t byte)
{
  static bool fCmd = false;
  uint8_t answer;
  
  if (fCmd == false)
  { // Обычный режим
    if (byte == 0x00)
    {
      fCmd = true;    // Перейти в режим байт-стаффинга
    }
    else
    {
      answer = spi1_rw(byte);  // Передать в карту принятый байт
      usart1_writeByte(answer);
    }
  }
  else
  { // Режим байт-стаффинга
    switch (byte)
    {
      case 0x00:
        answer = spi1_rw(byte);  // Передать в карту байт 0x00
        usart1_writeByte(answer);
        break;
      
      case 0x10: SPI1_NCS_0(); break;
      case 0x20: SPI1_NCS_1(); break;

      case 0x11: YLED_ON(); break;
      case 0x21: YLED_OFF(); break;

      case 0x12: BUZZER_ON(); break;        
      case 0x22: BUZZER_OFF(); break;      
        
      case 0x30: RLED1_ON(); break;
      case 0x40: RLED1_OFF(); break;

      case 0x31: GLED1_ON(); break;
      case 0x41: GLED1_OFF(); break;

      case 0x32: YLED1_ON(); break;
      case 0x42: YLED1_OFF(); break;

      case 0x33: RLED2_ON(); break;
      case 0x43: RLED2_OFF(); break;

      case 0x34: GLED2_ON(); break;
      case 0x44: GLED2_OFF(); break;

      case 0x35: YLED2_ON(); break;
      case 0x45: YLED2_OFF(); break;

      case 0x36: RLED3_ON(); break;
      case 0x46: RLED3_OFF(); break;

      case 0x37: RLED4_ON(); break;
      case 0x47: RLED4_OFF(); break;
        
      default:
        break;
    }
    fCmd = false;  // Перейти в обычный режим
  }
}


   
int main(void)
{
  uint8_t byte;
  
  systick_init();

  __enable_irq();  // Разрешить прерывания  

  gpios_init();
  gpios_testing();
  
  usart1_init();
  spi1_init();

  while (true)
  {
    if (usart1_readByte(&byte))
    { // Принят байт
      cmd_crack(byte);
      // usart1_writeByte(byte);      
    }
    // usart1_send((uint8_t *) hello, strlen(hello));
    // printf("Hi there!\n");
    // RLED1_TOGGLE();
    // delay_ms(100);
  }
}

