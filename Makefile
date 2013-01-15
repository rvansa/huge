CC=g++
NAME=huge
SRC=src
BIN=bin
FILES=\
	$(SRC)/main.cpp \
	$(SRC)/Command.cpp \
	$(SRC)/Display.cpp \
	$(SRC)/File.cpp \
	$(SRC)/Formatter.cpp \
	$(SRC)/Huge.cpp
	

all:
	g++ -g -Wall $(FILES) -o $(BIN)/$(NAME) -lncurses -I $(SRC) -std=c++11