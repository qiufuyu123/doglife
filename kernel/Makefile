BUILD_DIR := ./build/
SRC_DIR = ./
INCLUDE_DIR = ./include/ 
CFLAGS := -ffreestanding -std=gnu99 -O1
SRCS = $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.c' -or -name '*.s')
OBJS = $(SRCS:%=$(BUILD_DIR)/%.o)

all: clean dirs kernel8.img

dirs:
	mkdir -p $(BUILD_DIR)
#	mkdir -p $(OUT_DIR)

$(BUILD_DIR)/%.s.o: %.s
	mkdir -p $(dir $@)
	aarch64-linux-gnu-as -I$(INCLUDE_DIR)  -c $< -o $@

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	aarch64-linux-gnu-gcc -I$(INCLUDE_DIR) -I ../qui/include/ $(CFLAGS)  -c $< -o $@

kernel8.img: dirs $(OBJS)
	aarch64-linux-gnu-ld $(OBJS) ../qui/libarmqui.a -O0 -T ./linker.ld -o kernel8.elf
	aarch64-linux-gnu-objcopy -O binary ./kernel8.elf ./kernel8.bin
	

clean:
	-rm ./build -rf
	-rm ./kernel8.bin ./kernel8.elf -rf