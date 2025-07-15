#include "scheduler.h"
#include "tasks.h"
#include "my_cmsis.h"

Global_state global_state = {.current_task = 1};

void setup_scheduler(void)
{
	__asm volatile("MOV R11, LR");
	setup_scheduler_stack((uint32_t)SCHED_STACK_START);
	setup_tasks();
	setup_thread_use_psp();
	setup_systick(); // keep it in the end to be sure
	__asm volatile(
		"MOV LR, R11 \n"
		"BX LR \n");
}

__attribute__((naked)) void setup_scheduler_stack(uint32_t stack_start)
{
	__asm volatile("MSR MSP, R0");
	__asm volatile("BX LR");
}

__attribute__((naked)) void setup_thread_use_psp(void)
{
	__asm volatile("LDR R0, =tasks_state"); //
	__asm volatile("ADD R0, R0, %0" ::"I"(TASK_SIZE_BYTES));
	__asm volatile("LDR R0, [R0]"); // R1 = tasks_state[1].SP
	__asm volatile("MSR PSP, R0");
	__asm volatile("MOV R0, #0x02");
	__asm volatile("MSR CONTROL, R0");
	__asm volatile("BX LR");
}

void setup_systick(void)
{
	// setup systick for 1 ms
	SYST_RVR = 0x1F3F; // 8000 - 1 for 8mhz
	SYST_CSR |= (1 << SYST_CLKSOURCE) | (1 << TICKINT);
	SYST_CSR |= (1 << SYST_ENABLE);
}

__attribute__((naked)) void SysTick_Handler(void)
{

	__asm volatile("PUSH {LR}");
	// do not use r4 - r11
	update_tick_count();

	unblock_tasks();

	set_pendSV();

	__asm volatile("POP {LR}");

	__asm volatile("BX LR");
}

void set_pendSV(void)
{
	ICSR |= (1 << PENDSVSET);
}

void update_tick_count(void)
{
	global_state.tick_count++;
}

void unblock_tasks(void)
{
	for (int i = 0; i < TASKS_NUM; i++)
	{
		if (tasks_state[i].status == TASK_STATUS_BLOCKED)
		{
			if (global_state.tick_count >= tasks_state[i].wake_up_time)
			{
				tasks_state[i].status = TASK_STATUS_READY;
			}
		}
	}
}

__attribute__((naked)) void PendSV_Handler(void)
{
	// save state
	__asm volatile(
		"MRS R0, PSP\n"			// get PSP
		"STMDB R0!, {R4-R11}\n" // store r4-r11 onto PSP
		"MSR PSP, R0\n"			// update PSP
	);

	__asm volatile("PUSH {LR}");
	// save stack pointer
	__asm volatile("BL save_sp");

	// decide next task
	__asm volatile("BL round_robin");

	// update global state
	__asm volatile("LDR R1, =global_state"); // global state.current_task
	__asm volatile("STR R0, [R1]");

	// load next task
	__asm volatile("BL load_sp");

	__asm volatile("POP {LR}");

	__asm volatile("MSR PSP, R0");

	__asm volatile(
		"LDMIA R0!, {R4-R11}\n"
		"MSR PSP, R0\n"
	);
	__asm volatile("BX LR");
}

void save_sp(uint32_t sp)
{
	tasks_state[global_state.current_task].SP = (uint32_t)sp;
}

uint32_t load_sp(void)
{
	return tasks_state[global_state.current_task].SP;
}

uint32_t round_robin()
{

	int status = TASK_STATUS_BLOCKED;
	for (int i = 0; i < TASKS_NUM; i++)
	{
		global_state.current_task++;
		global_state.current_task %= TASKS_NUM;
		status = tasks_state[global_state.current_task].status;
		if (status == TASK_STATUS_READY && global_state.current_task != 0)
			break;
	}

	if (status != TASK_STATUS_READY)
	{
		global_state.current_task = IDLE_TAKS;
	}

	return global_state.current_task;
}

void start_first_task(uint32_t task_id)
{
	global_state.current_task = task_id;
	uint32_t task_sp = tasks_state[task_id].SP;
	__asm volatile("LDR R0, %0" ::""(task_sp));
	__asm volatile("MSR PSP, R0");
	__asm volatile("POP {R4-R11}");
	__asm volatile("POP {R0,R1,R2,R3,R12,LR}");
	__asm volatile("POP {LR}");
	__asm volatile("POP {R0}");
	__asm volatile("BX LR");
}

void this_task_sleep(uint32_t tick_count)
{
	task_sleep(global_state.current_task, tick_count);
	set_pendSV();
}

void task_sleep(uint32_t task_id, uint32_t tick_count)
{
	// avoid race condition
	INTERRUPT_DISABLE();
	if (task_id == IDLE_TAKS)
	{
		// could trigger custom exception
		return;
	}
	tasks_state[task_id].status = TASK_STATUS_BLOCKED;
	tasks_state[task_id].wake_up_time = global_state.tick_count + tick_count;
	INTERRUPT_ENABLE();
}
