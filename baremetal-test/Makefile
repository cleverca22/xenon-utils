all: kernel.elf kernel.dis kernel.syms kernel.bin

kernel.elf: boot.o cache.o main.o
	$(LD) -o $@ $^ -nostdlib -T kernel.ld

kernel.dis: kernel.elf
	$(OBJDUMP) -d $< > $@

kernel.syms: kernel.elf
	$(OBJDUMP) -t $< > $@

kernel.bin: kernel.elf
	$(OBJCOPY) -O binary $< $@

%.o: %.c
	$(CC) -c -o $@ $< -nostdlib

%.o: %.S
	$(CC) -c -o $@ $< -nostdlib

install: kernel.elf kernel.dis kernel.syms kernel.bin
	mkdir -pv $(out)/bin/
	cp -v $^ $(out)/bin/
