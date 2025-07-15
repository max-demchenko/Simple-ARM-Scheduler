#include <stdint.h>

#ifndef TASK_H
#define TASK_H

#define DELAY_COUNT_1S 		1000
#define DELAY_COUNT_500MS 	500
#define DELAY_COUNT_250MS	250
#define DELAY_COUNT_125MS	125

#define TASK_STATUS_READY 1
#define TASK_STATUS_BLOCKED 0

#define TASK_STACK_SIZE 1024U
#define SCHED_STACK_SIZE 1024U

#define SRAM_START (uint32_t) 0x20000000
#define SRAM_SIZE (10 * 1024)
#define SRAM_END  SRAM_START + SRAM_SIZE

#define TASK1_STACK_START SRAM_END
#define TASK2_STACK_START TASK1_STACK_START - TASK_STACK_SIZE
#define TASK3_STACK_START TASK2_STACK_START - TASK_STACK_SIZE
#define TASK4_STACK_START TASK3_STACK_START - TASK_STACK_SIZE
	//add new tasks here
#define IDLE_TASK_STACK_START TASK4_STACK_START - TASK_STACK_SIZE
#define SCHED_STACK_START IDLE_TASK_STACK_START - 32 * 4

#define TASKS_NUM 4 + 1// idle task
#define IDLE_TAKS 0
#define TASK_1 1
#define TASK_2 2
#define TASK_3 3
#define TASK_4 4

#define TASK_STACK_FRAME_SIZE 16

void setup_tasks_state(void);

void setup_tasks_stack(void);

void setup_tasks(void);

void start_first_task(uint32_t task_num);


typedef struct {
	uint32_t SP;
	uint32_t task_handler;
	uint32_t wake_up_time;
	uint8_t status;
} Task_state;


#define TASK_SIZE_BYTES (sizeof(Task_state))



extern Task_state tasks_state[TASKS_NUM];


void idle_task(void);
void task1(void);
void task2(void);
void task3(void);
void task4(void);



#endif



