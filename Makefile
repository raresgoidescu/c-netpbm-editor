# Copyright Rares-Stefan Goidescu 312CAb 2023-2024

CC=gcc
FLAGS=-Wall -Wextra -std=c99
TARGETS=

build: $(TARGETS)
	$(CC) $(FLAGS) -o   $(TARGETS) -g

run:
	$(CC) $(FLAGS) -o   $(TARGETS) -g
	valgrind ./


clean:

pack:
	zip -FSr 312CA_GoidescuRares-Stefan_Tema3.zip README Makefile *.c *.h
