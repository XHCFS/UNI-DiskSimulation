CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LIBS = -lrt

SOURCES = disk.c kernel.c userProcess.c
HEADERS = common_module.h disk.h kernel.h userProcess.h

BINARIES = disk.out kernel.out userprocess.out

all: $(BINARIES)

disk.out: disk.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

kernel.out: kernel.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

userprocess.out: userProcess.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

run-all:
	st -e ./disk.out & \
	st -e ./kernel.out & \
	st -e ./userprocess.out &

clean:
	rm -f $(BINARIES)

.PHONY: all run-all clean
