BUILD_DIR := ./build/
SRC_DIR = ./
INCLUDE_DIR = ./include/
CFLAGS := -std=gnu99 -O1 -static
SRCS = $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.c' -or -name '*.s')
OBJS = $(SRCS:%=$(BUILD_DIR)/%.o)

all: clean dirs libqui.a

dirs:
	mkdir -p $(BUILD_DIR)
#	mkdir -p $(OUT_DIR)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	aarch64-linux-gnu-gcc -I$(INCLUDE_DIR) $(CFLAGS)  -c $< -o $@

libqui.a: dirs $(OBJS)
	ar -rcs ./libarmqui.a $(OBJS) 


clean:
	-rm ./build -rf
	-rm ./*.a 