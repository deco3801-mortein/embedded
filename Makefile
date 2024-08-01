CC = gcc
CFLAGS = -Wall -pedantic -std=gnu99

EXECS = hello

.PHONY = clean
.DEFAULT_GOAL := all

all: ${EXECS}

${EXECS}: %: src/%.c
	${CC} ${CFLAGS} $< -o bin/$@

clean:
	@rm -f bin/${EXECS}
