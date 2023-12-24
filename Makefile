# Copyright Rares-Stefan Goidescu 312CAb 2023-2024

CC=gcc
FLAGS=-Wall -Wextra -std=c99
TARGETS=main.c

build: $(TARGETS)
	$(CC) $(FLAGS) -o image_editor  $(TARGETS) -g

run:
	$(CC) $(FLAGS) -o image_editor  $(TARGETS) -g
	valgrind ./image_editor

clean:
	rm -f image_editor

pack:
	zip -FSr 312CA_GoidescuRares-Stefan_Tema3.zip README Makefile *.c *.h
