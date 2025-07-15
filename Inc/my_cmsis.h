#include <stdint.h>

#ifndef MY_CMSIS_H
#define MY_CMSIS_H



//systick stuff
#define SYST_CLKSOURCE 2
#define TICKINT 1
#define SYST_ENABLE 0

#define SYST_CSR *(volatile uint32_t *)0xE000E010
#define SYST_RVR *(volatile uint32_t *)0xE000E014
#define SYST_CVR *(volatile uint32_t *)0xE000E018
#define SYST_CALIB *(volatile uint32_t *)0xE000E01c


//pendsv

#define PENDSVSET 28

#define ICSR *(volatile uint32_t *)0xE000ED04


//io
#define RCC_APB2ENR   (*(volatile unsigned int*)0x40021018)
#define GPIOB_CRH     (*(volatile unsigned int*)0x40010C04)
#define GPIOB_ODR     (*(volatile unsigned int*)0x40010C0C)
#define GPIOB_BSRR    (*(volatile unsigned int*)0x40010C10)
#define GPIOB_BRR     (*(volatile unsigned int*)0x40010C14)



#endif
