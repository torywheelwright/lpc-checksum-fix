CC     := gcc
CFLAGS :=

.PHONY: all
all: lpc-checksum-fix

lpc-checksum-fix: lpc-checksum-fix.c

.PHONY: clean
clean:
	rm -f lpc-checksum-fix
