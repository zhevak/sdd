// usart1.h


#ifndef __USART1_H__
#define __USART1_H__


#include <stdbool.h>



void usart1_init(void);

bool usart1_readByte(uint8_t *byte);
void usart1_writeByte(uint8_t byte);



#endif  /* __USART1_H__ */
