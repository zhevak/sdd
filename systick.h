// systick.h

#ifndef __SYSTICK_H__
#define __SYSTICK_H__


#ifdef __cplusplus
 extern "C" {
#endif


void systick_init(void);
void SysTick_Handler(void);

void delay_ms(uint32_t dt);  // Тупая задержка


#ifdef __cplusplus
}
#endif



#endif
