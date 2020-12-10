#
# Makefile for MPX Modules

AS      = nasm
CC      = i386-elf-gcc
CFLAGS  = -Wall -Wextra -Werror -nostartfiles -nostdlib -nodefaultlibs -ffreestanding -g -c
LD      = i386-elf-ld
LDFLAGS = 
ASFLAGS = -f elf -g

# add new files as you create them.
# I'll start you off with just mpx_supt.o
OBJFILES =\
mpx_supt.o \
R1/comhand.o \
R2/pcb.o \
R2/queue.o \
R2/processManagement.o \
R1/time.o \
R1/date.o \
R2/R2Commands.o \
R1/miscCommands.o \
R3/context.o \
R3/test.o \
R4/loadComhand.o\
R4/alarm.o\
R4/list.o\
R5/memoryManagment.o\
R5/mList.o\
R5/memoryCommands.o\
R6/IOqueue.o\
R6/driver.o

.c.s:
	$(CC) $(CFLAGS) -S -o $@ $<
.s.o:
	$(AS) $(ASFLAGS) -o $@ $<
.c.o:
	$(CC) $(CFLAGS) -I../include -o $@ $<

all: modules.o

modules.o: $(OBJFILES)
	$(LD) -r -o modules.o $(OBJFILES)

clean:
	rm modules.o $(OBJFILES)
