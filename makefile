PP = g++
OBJ := main.o Util.o ThreadPool.o HttpData.o DayList.o DayListUser.o 

PTHREAD := -pthread
WALL = -Wall
MYSQL = `mysql_config --cflags --libs`
LIBS = $(shell mysql_config --libs)
CFLAGS = $(shell mysql_config --cflags)

TOP_DIR := $(PWD)
OBJ_DIR := $(TOP_DIR)/obj
BIN_DIR := $(TOP_DIR)/bin
BIN := main
SUB_DIR := src \
			src/mysql \
			src/API \
			obj
export PP SRC INCLUDE OBJ PTHREAD MYSQL TOP_DIR OBJ_DIR BIN_DIR BIN WALL LIBS CFLAGS

all : CHECKDIR $(SUB_DIR)
CHECKDIR : 
	mkdir -p $(SUB_DIR) $(BIN_DIR)
$(SUB_DIR) : ECHO
	make -C $@
ECHO : 
	@echo $(SUB_DIR)
	@echo begin complile

clean : 
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(BIN_DIR)/main