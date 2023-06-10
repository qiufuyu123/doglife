.PHONY: kernel emu

kernel:
	make -C qui -f arm.mk
	make -C kernel
emu:
	make -C qui
	make -C emu
	./emu/emu
clean:
	make -C kernel clean
	make -C qui clean
	make -C emu clean
	