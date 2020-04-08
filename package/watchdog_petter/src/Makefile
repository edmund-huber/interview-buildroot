CC = gcc

.PHONY: clean

all: watchdog_petter

watchdog_petter: watchdog_petter.c
	$(CC) -o "$@" "$<"

clean:
	rm *.o
	rm watchdog_petter
