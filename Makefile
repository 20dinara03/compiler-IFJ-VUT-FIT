# directory mapping
PROJ_D=$(shell pwd)
SRC_D=src
BIN_D=bin
TESTS_D=tests
# compiler config
CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c99

run:
	mkdir -p $(BIN_D)
	$(CC) $(CFLAGS) $(shell find ./src -name '*.c') -o ifj22