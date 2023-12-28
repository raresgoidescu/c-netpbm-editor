# Copyright Rares-Stefan Goidescu 312CAb 2023-2024

CC=gcc
FLAGS=-Wall -Wextra -std=c99
TARGETS=main.c

build: $(TARGETS)
	$(CC) $(FLAGS) $(TARGETS) -lm -o image_editor -g

run:
	$(CC) $(FLAGS) $(TARGETS) -lm -o image_editor -g
	valgrind --leak-check=full ./image_editor

clean:
	rm -f image_editor

pack:
	zip -FSr 312CA_GoidescuRares-Stefan_Tema3.zip README Makefile *.c *.h
