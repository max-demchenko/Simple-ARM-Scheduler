#include "stdint.h"


#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct {
	uint32_t current_task;
	uint32_t tick_count;
} Global_state;


extern Global_state global_state;



#define INTERRUPT_DISABLE() do {__asm volatile("MOV R0, #0x1"); __asm volatile("MSR PRIMASK, R0");} while(0);
#define INTERRUPT_ENABLE() do {__asm volatile("MOV R0, #0x0"); __asm volatile("MSR PRIMASK, R0");} while(0);

void setup_scheduler(void);

void setup_scheduler_stack(uint32_t stack_start);

__attribute__ ((naked)) void setup_scheduler_stack(uint32_t stack_start);

__attribute__ ((naked)) void setup_thread_use_psp(void);

uint32_t round_robin();

void setup_systick();

void set_pendSV(void);

void update_tick_count(void);

void unblock_tasks(void);

void start_first_task(uint32_t task_id);

void this_task_sleep(uint32_t tick_count);

void task_sleep(uint32_t task_id, uint32_t tick_count);


#endif



