# Makefile

#PATH	    := C:\MinGW\bin

CC	    := gcc
#ARCH	    := -m32
EXE_RX	    := arinc_box_rx.exe
EXE_TX	    := arinc_box_tx.exe

# ------------------------------------------------------------------------------

default: compile

# C-Compiler flags
#
CFLAGS      :=  -c -std=gnu99 ${ARCH} \
		-Wall ${SERIAL_DEBUG} \
		-O3 -g0 

# Linker flags (-s: strip)
LFLAGS      :=  -s

SOURCES_TX	    := main_tx.c serial.c arinc_box_translator.c
SOURCES_RX	    := main_rx.c serial.c arinc_box_translator.c

OBJECTS_TX := ${SOURCES_TX:.c=.o}
OBJECTS_TX := ${OBJECTS_TX:.S=.o}


OBJECTS_RX := ${SOURCES_RX:.c=.o}
OBJECTS_RX := ${OBJECTS_RX:.S=.o}

%.o: %.c
	${CC} ${CFLAGS}  $< -o $@

%.o: %.S
	${CC} ${AFLAGS}  $< -o $@

${EXE_TX}: ${OBJECTS_TX}
	${CC} ${LFLAGS} ${OBJECTS_TX} -o $@

${EXE_RX}: ${OBJECTS_RX}
	${CC} ${LFLAGS} ${OBJECTS_RX} -o $@

# ------------------------------------------------------------------------------

compile_tx: clean ${EXE_TX}

compile_rx: clean ${EXE_RX}

# ------------------------------------------------------------------------------

.PHONY: clean
clean:
	del *.o

