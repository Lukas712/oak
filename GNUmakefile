# Configurações do Projeto Oak (C)
CC := gcc
CFLAGS := -g -O2 -Wall -Wextra -std=gnu11 -ffreestanding -mno-red-zone -m64 -mcmodel=kernel -I src/init
LDFLAGS := -m elf_x86_64 -nostdlib -T linker.ld
IMAGE_NAME := oak

.PHONY: all
all: $(IMAGE_NAME).iso

# 1. Baixa o protocolo oficial via git clone
src/init/limine.h:
	rm -rf limine-protocol-tmp
	git clone https://github.com/limine-bootloader/limine-protocol.git limine-protocol-tmp
	cp limine-protocol-tmp/include/limine.h src/init/limine.h
	rm -rf limine-protocol-tmp

# 2. Baixa e compila o utilitário binário do Limine para criar a ISO
.INTERMEDIATE: limine-binary.tar.gz
limine-binary.tar.gz:
	curl -fL -o $@ https://github.com/Limine-Bootloader/Limine/releases/latest/download/limine-binary.tar.gz

limine-binary/limine: limine-binary.tar.gz
	rm -rf limine-binary
	gunzip < limine-binary.tar.gz | tar -xf -
	$(MAKE) -C limine-binary

# 3. Compila o Kernel C
kernel.elf: src/init/limine.h src/init/main.c linker.ld
	$(CC) $(CFLAGS) -c src/init/main.c -o main.o
	ld $(LDFLAGS) main.o -o kernel.elf

# 4. Empacota tudo em uma imagem ISO bootável
$(IMAGE_NAME).iso: limine-binary/limine kernel.elf
	rm -rf iso_root
	mkdir -p iso_root/boot/limine
	cp kernel.elf iso_root/boot/oak.elf
	cp limine.conf iso_root/                                                                 # <-- CORRIGIDO AQUI (vai para a raiz da ISO)
	cp limine-binary/limine-bios.sys limine-binary/limine-bios-cd.bin limine-binary/limine-uefi-cd.bin iso_root/boot/limine/
	mkdir -p iso_root/EFI/BOOT
	cp limine-binary/BOOTX64.EFI limine-binary/BOOTIA32.EFI iso_root/EFI/BOOT/
	xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		iso_root -o $(IMAGE_NAME).iso
	./limine-binary/limine bios-install $(IMAGE_NAME).iso
	rm -rf iso_root main.o

# 5. Roda no QEMU redirecionando a porta serial para o seu terminal
.PHONY: run
run: $(IMAGE_NAME).iso
	qemu-system-x86_64 \
		-M q35 \
		-m 512M \
		-cdrom $(IMAGE_NAME).iso \
		-serial stdio \
		-no-reboot \
		-no-shutdown

# Limpeza dos artefatos de build
.PHONY: clean
clean:
	rm -rf iso_root *.iso kernel.elf main.o limine-binary limine-binary.tar.gz src/init/limine.h