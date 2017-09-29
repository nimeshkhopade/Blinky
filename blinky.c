#include <stm32f4xx.h>
long test, PD12, PD13, PD14, PD15, PA8;
int i = 0, count = 0;
void delay(){
	int j = 0;
	for(i = 0;i < 50000; i++)
	{
		for(j = 0; j < 500; j++){}
		//count++;
	}
}

int main(void)
{
	/* Flash settings (see RM0090 rev9, p80) */

	FLASH->ACR =
            FLASH_ACR_LATENCY_5WS               // 6 CPU cycle wait 
          | FLASH_ACR_PRFTEN                    // enable prefetch 
          | FLASH_ACR_ICEN                      // instruction cache enable *
          | FLASH_ACR_DCEN;                     // data cache enable 
	
	/* Configure clocks
	 * Max SYSCLK: 168MHz
	 * Max AHB:  SYSCLK
	 * Max APB1: SYSCLK/4 = 48MHz
	 * Max APB2: SYSCLK/2 = 86MHz
	 * + enable sys clock output 2 with clock divider = 4 
	*/
	/*
	RCC->CFGR =
          0x0                           // Clock output 2 is SYSCLK (RCC_CFGR_MCO2) 
        | ( 0x6 << 27)                  // Clock output divider 
        | RCC_CFGR_PPRE2_DIV2           // APB2 prescaler 
        | RCC_CFGR_PPRE1_DIV4;          // APB2 prescaler 
				*/
	
	test = RCC->CFGR; //0x30009400
	test = RCC->CR; //0x00008B83
	
	/* Clock control register */
	RCC->CR = RCC_CR_HSEON;         /* Enable external oscillator */

	/* Wait for locked external oscillator */
	while((RCC->CR & RCC_CR_HSERDY) != RCC_CR_HSERDY);
	test = RCC->CR; //0x00037B03
	
	/* PLL config */
	RCC->PLLCFGR =
          RCC_PLLCFGR_PLLSRC_HSE                /* PLL source */
        | (4 << 0)                              /* PLL input division */
        | (168 << 6)                            /* PLL multiplication */
        | (0 << 16)                             /* PLL sys clock division */
        | (7 << 24);                            /* PLL usb clock division =48MHz */

	/* crystal:  8MHz
	 * PLL in:   2MHz (div 4)
	 * PLL loop: 336MHz (mul 168)
	 * PLL out:  168MHz (div 2)
	 * PLL usb:  48MHz (div 7)
	*/
	test = RCC->PLLCFGR; //0x07402A04
	
	RCC->CR |= RCC_CR_PLLON;		/* Enable PLL */

	while((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);	/* Wait for locked PLL */
	
	test = RCC->CR; //0x03037B03
	
	RCC->CFGR &= ~RCC_CFGR_SW; 		/* select system clock */			/* clear */
	
	test = RCC->CFGR; 	//0x30009400
	
	//RCC->AHB1ENR |= 1;
	
	//GPIOA->MODER |= (2 << 16);
	
	//RCC->CFGR |= (6 << 24) | (3 << 21);
	
	RCC->CFGR |= RCC_CFGR_SW_PLL | RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2;   /* SYSCLK is PLL */
	
	test = RCC->CFGR; //0x3000940A
	
	while((RCC->CFGR & RCC_CFGR_SW_PLL) != RCC_CFGR_SW_PLL);		/* Wait for SYSCLK to be PLL */
	
	//while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) //Wait for config to take place
	
	test = RCC->CFGR; //0x3000940A

	RCC->AHB1ENR |= 1 << 3;
	
	GPIOD->MODER   = GPIO_MODER_MODE15_0|GPIO_MODER_MODE14_0|GPIO_MODER_MODE13_0|GPIO_MODER_MODE12_0;		//0x55 << 16; /* output */
	
	int x = 12;
	
	while(1){
	
	/* PC9 = MCO2 = alternate func */
	
	//PA8 = GPIOA->ODR;
	
	/*GPIOC->OTYPER  = 0x00000000; // push-pull 
	GPIOC->OSPEEDR = 0x000D0000; // medium speed 
	*/
	//GPIOD->ODR = 0x0000;
	delay();
	GPIOD->ODR ^= 0xf000;
	delay();
		delay();
	GPIOD->ODR ^= GPIOD->ODR;
		delay();
	//x++;
	for(int k = 0; k < 4; k++){
		delay();
		GPIOD->ODR ^= 1 << x;
		PD12 = GPIOD->ODR & 0x00001000;
		PD13 = GPIOD->ODR & 0x00002000;
		PD14 = GPIOD->ODR & 0x00004000;
		PD15 = GPIOD->ODR & 0x00008000;
		test = GPIOD->ODR;
		delay();
		x++;
		count++;
	}
	GPIOD->ODR ^= GPIOD->ODR;
	for(int k = 0; k < 4; k++){
		x--;
		delay();
		GPIOD->ODR ^= 1 << x;
		PD12 = GPIOD->ODR & 0x00001000;
		PD13 = GPIOD->ODR & 0x00002000;
		PD14 = GPIOD->ODR & 0x00004000;
		PD15 = GPIOD->ODR & 0x00008000;
		test = GPIOD->ODR;
		delay();
		count++;
		//x--;
	}
	GPIOD->ODR ^= GPIOD->ODR;
	count++;
} 
}
