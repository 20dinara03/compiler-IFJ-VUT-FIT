# directory mapping
PROJ_D=$(shell pwd)
SRC_D=src
BIN_D=bin
TESTS_D=tests
# compiler config
CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c99
# packing config
TAR_NAME=project


test:
	echo "Testing here"

compile:
	mkdir -p $(BIN_D)
	$(CC) $(CFLAGS) $(wildcard $(PROJ_D)/$(SRC_D)/*.c) -o $(BIN_D)/$@

pack:
	tar -czvf $(TAR_NAME).tar.gz $(SRC_D) $(BIN_D) README.md

clean:
	rm $(PROJ_D)/$(BIN_D)/*
	rm -f $(PROJ_D)/$(TAR_NAME).tar.gz
