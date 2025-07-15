#include "tasks.h"
#include "my_cmsis.h"
#include "scheduler.h"

Task_state tasks_state[TASKS_NUM];

void setup_tasks(void)
{
	setup_tasks_state();
	setup_tasks_stack();
}

void setup_tasks_state(void)
{
	tasks_state[0].task_handler = (uint32_t)&idle_task | 0x1;
	tasks_state[1].task_handler = (uint32_t)&task1 | 0x1;
	tasks_state[2].task_handler = (uint32_t)&task2 | 0x1;
	tasks_state[3].task_handler = (uint32_t)&task3 | 0x1;
	tasks_state[4].task_handler = (uint32_t)&task4 | 0x1;

	tasks_state[0].status = TASK_STATUS_READY;
	tasks_state[1].status = TASK_STATUS_READY;
	tasks_state[2].status = TASK_STATUS_READY;
	tasks_state[3].status = TASK_STATUS_READY;
	tasks_state[4].status = TASK_STATUS_READY;

	tasks_state[0].SP = IDLE_TASK_STACK_START;
	tasks_state[1].SP = TASK1_STACK_START;
	tasks_state[2].SP = TASK2_STACK_START;
	tasks_state[3].SP = TASK3_STACK_START;
	tasks_state[4].SP = TASK4_STACK_START;
}

void setup_tasks_stack(void)
{
	for (register uint32_t i = 0; i < TASKS_NUM; i++)
	{

		uint32_t *sp = (uint32_t *)tasks_state[i].SP;
		// PSR
		*(sp - 1) = 0x01000000; // make sure the t bit is set
		*(sp - 2) = (uint32_t)tasks_state[i].task_handler;
		*(sp - 3) = 0xfffffffd; // magic number for returning form handler to thread in psp
		for (register uint32_t j = 4; j <= 16; j++)
		{
			*(sp - j) = 0x00000000;
		};

		tasks_state[i].SP = (uint32_t)(sp - TASK_STACK_FRAME_SIZE);
	};
}

void idle_task(void)
{
	while (1)
		;
}

void task1(void)
{
	while (1)
	{
		GPIOB_BRR |= (0x1 << 12);
		this_task_sleep(DELAY_COUNT_1S);
		GPIOB_BSRR |= (0x1 << 12);
		this_task_sleep(DELAY_COUNT_1S);
	}
}

void task2(void)
{
	while (1)
	{
		GPIOB_BRR |= (0x1 << 13);
		this_task_sleep(DELAY_COUNT_500MS);
		GPIOB_BSRR |= (0x1 << 13);
		this_task_sleep(DELAY_COUNT_500MS);
	}
}

void task3(void)
{
	while (1)
	{
		GPIOB_BRR |= (0x1 << 14);
		this_task_sleep(DELAY_COUNT_250MS);
		GPIOB_BSRR |= (0x1 << 14);
		this_task_sleep(DELAY_COUNT_250MS);
	}
}

void task4(void)
{
	while (1)
	{
		GPIOB_BRR |= (0x1 << 15);
		this_task_sleep(DELAY_COUNT_125MS);
		GPIOB_BSRR |= (0x1 << 15);
		this_task_sleep(DELAY_COUNT_125MS);
	}
}
