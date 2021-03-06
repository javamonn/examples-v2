/*
 *  LED -- simple LED support
 */

#ifndef __LED_h
#define __LED_h

#define __LONG_LED

#include <stdio.h>

#if defined(mbx860)

#define MBX_LED_REG *((volatile unsigned char *)0xfa100001)

#define LED_ON()  MBX_LED_REG = 0x0F
#define LED_OFF() MBX_LED_REG = 0x00

#elif defined(icecube)

#include <bsp/mpc5200.h>

#define LED_INIT() \
  do { \
    mpc5200.gpiosen |= 0x30000000; \
    mpc5200.gpiosdd |= 0x30000000; \
  } while (0)

#define LED_ON() \
  do { \
    mpc5200.gpiosdo |= 0x30000000; \
  } while (0)

#define LED_OFF() \
  do { \
    mpc5200.gpiosdo &= ~0x30000000; \
  } while (0)

#elif defined(MPC8313ERDB)

/* This rotates a single LED off around the back of 8. */
volatile uint8_t *MPC8313_LED_REG;
uint8_t MPC8313_LED_Values[] =
    { 0x01, 0x02, 0x04, 0x08, 0x80, 0x40, 0x20, 0x10};
uint8_t MPC8313_LED_Count;

#define LED_INIT()  \
  do { \
    MPC8313_LED_REG = (uint8_t *) 0xfa000000; \
    MPC8313_LED_Count = 0; \
  } while (0)

#define LED_ROTATE()  \
  do { \
    *MPC8313_LED_REG = MPC8313_LED_Values[MPC8313_LED_Count]; \
    /* printk( "v=0x%02x ", MPC8313_LED_Values[MPC8313_LED_Count] ); */ \
    if ( ++MPC8313_LED_Count == 9 ) \
      MPC8313_LED_Count = 0; \
  } while (0)

#define LED_ON() LED_ROTATE()
#define LED_OFF() LED_ROTATE()

#elif defined(BCM2835_GPIO_REGS_BASE)
// Raspberry Pi
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3)) 
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3)) 
#define GPIO_SET *(gpio+7)   // sets   bits which are 1 ignores bits which are 0 
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define LED_INIT()  do { unsigned int *gpio = (unsigned int *)BCM2835_GPIO_REGS_BASE; OUT_GPIO(16);} while(0)
#define LED_ON()  do { unsigned int *gpio = (unsigned int *)BCM2835_GPIO_REGS_BASE; GPIO_CLR = 1 << 16;} while(0)
#define LED_OFF()  do { unsigned int *gpio = (unsigned int *)BCM2835_GPIO_REGS_BASE; GPIO_SET = 1 << 16;} while(0)

#else
/* default case is to print */

#define __LED_PRINTING 1
#define LED_ON()  fputs( "LED ON\n", stderr )
#define LED_OFF() fputs( "LED OFF\n", stderr )
#endif

#ifndef LED_INIT
#define LED_INIT() 
#endif

#endif
