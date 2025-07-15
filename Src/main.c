#include "tasks.h"
#include "my_cmsis.h"
#include "scheduler.h"

void init_io(void)
{
	RCC_APB2ENR |= (1 << 3);

	// 2. Set PB12 as output push-pull, 2 MHz
	GPIOB_CRH &= ~(
		(0xF << 16) | // PB12
		(0xF << 20) | // PB13
		(0xF << 24) | // PB14
		(0xF << 28)	  // PB15
	);

	// Set each to 0b0010: output, 2 MHz, push-pull
	GPIOB_CRH |= ((0x2 << 16) | // PB12
				  (0x2 << 20) | // PB13
				  (0x2 << 24) | // PB14
				  (0x2 << 28)	// PB15
	);
}

int main(void)
{
	init_io();

	setup_scheduler();

	start_first_task(TASK_1);

	while (1)
	{
	}
}
