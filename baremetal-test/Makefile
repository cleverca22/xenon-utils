all: kernel.elf kernel.dis kernel.bin

kernel.elf: boot.o main.o
	$(LD) -o $@ $^ -nostdlib -T kernel.ld

kernel.dis: kernel.elf
	$(OBJDUMP) -d $< > $@

kernel.bin: kernel.elf
	$(OBJCOPY) -O binary $< $@

%.o: %.c
	$(CC) -c -o $@ $< -nostdlib

%.o: %.S
	$(CC) -c -o $@ $< -nostdlib

install: kernel.elf kernel.dis kernel.bin
	mkdir -pv $(out)
	cp -v $^ $(out)
