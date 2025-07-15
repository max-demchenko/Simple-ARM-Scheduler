CC=arm-none-eabi-gcc
MACHINE=cortex-m3 
OPTIMIZATION=-O0 -g
CFLAGS= -IInc -Wall -c -mcpu=$(MACHINE) $(OPTIMIZATION) -mthumb -std=gnu11 
LDFLAGS= -nostdlib -T linker_script.ld -Wl,-Map=Build/final.map



#$^ dependency
#$@ target
SRC := $(wildcard Src/*.c)
OBJ := $(patsubst Src/%.c, Build/%.o, $(SRC))


#/usr/share/openocd/scripts/target/stm32f1x.cfg


flash: final.elf
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg

final.elf: all
	$(CC) $(LDFLAGS) ./Build/*.o -o ./Build/final.elf

all: clean $(OBJ) startup_stm32f103c6t6.o

Build/%.o: Src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

startup_stm32f103c6t6.o: startup_stm32f103c6t6.c
	$(CC) $(CFLAGS) $^ -o ./Build/$@


# main.o: ./Src/main.c
# 	$(CC) $(CFLAGS) $^ -o ./Build/$@

# tasks.o: ./Src/tasks.c
# 	$(CC) $(CFLAGS) $^ -o ./Build/$@

# scheduler.o: ./Src/scheduler.c
# 	$(CC) $(CFLAGS) $^ -o ./Build/$@

#clean
clean:
	rm -f ./Build/*.o
