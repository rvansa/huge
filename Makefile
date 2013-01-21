CC=g++
CFLAGS=-g -rdynamic -Wall -Wextra -std=c++11
LIBS=-lpthread -lncurses
SRC=src
BIN=bin
INCLUDES=-I $(SRC) -I tre
TRE=tre/libtre.a
NAME=huge
FILES=\
	$(SRC)/main.cpp \
	$(SRC)/Command.cpp \
	$(SRC)/Display.cpp \
	$(SRC)/Exception.cpp \
	$(SRC)/File.cpp \
	$(SRC)/Formatter.cpp \
	$(SRC)/Huge.cpp \
	$(SRC)/MultiRegexp.cpp \
	$(SRC)/Job.cpp \
	$(SRC)/SearchJob.cpp \
	$(SRC)/IndexJob.cpp
	

all:
	g++ $(CFLAGS) $(INCLUDES) $(LIBS) $(FILES) $(TRE) -o $(BIN)/$(NAME)  	
	
clean:
	rm $(BIN)/*
