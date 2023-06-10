BUILD_DIR := ./build/
SRC_DIR = ./
INCLUDE_DIR = ../qui/include
CFLAGS := -std=gnu99 -O1 -static
SRCS = $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.c' -or -name '*.s')
OBJS = $(SRCS:%=$(BUILD_DIR)/%.o)


all: 
	gcc  $(SRCS) -Wl,-Bstatic ../qui/libqui.a -I $(INCLUDE_DIR)  -DEMU=1 -Wl,-Bdynamic -lSDL2 -lSDL2main -o emu
clean:
	-rm ./emu -rf