GCC=/home/mateusz/opt/cross/bin/i686-elf-gcc
LD=/home/mateusz/opt/cross/bin/i686-elf-ld
AR=/home/mateusz/opt/cross/bin/i686-elf-ar
OBJCOPY=/home/mateusz/opt/cross/bin/i686-elf-objcopy
GCC_FLAGS=-Wall -c -ffreestanding -fno-pie
ISOGEN=genisoimage
BUILDDIR=build
IMG_DIR=${BUILDDIR}/image

all: ${BUILDDIR}/bootloader.iso

${BUILDDIR}/%.o: %.S
	${GCC} ${GCC_FLAGS} $< -o $@

${BUILDDIR}/boot.elf: ${BUILDDIR}/boot.o
	${LD} ${KERNEL_LDFLAGS} -Ttext 0 $< -o $@

${BUILDDIR}/bootloader.bin: ${BUILDDIR}/boot.elf
	${OBJCOPY} -O binary $< $@

${BUILDDIR}/kernelcore.img: ${BUILDDIR}/bootloader.bin
	cat ${BUILDDIR}/bootloader.bin /dev/zero | head -c 2048 > $@

${BUILDDIR}/bootloader.iso: ${BUILDDIR}/kernelcore.img
	rm -rf ${IMG_DIR}
	mkdir ${IMG_DIR} ${IMG_DIR}/boot
	mv ${BUILDDIR}/kernelcore.img ${IMG_DIR}/boot
	${ISOGEN} -no-emul-boot -boot-load-size 4 -input-charset utf-8 -iso-level 2 -J -R -o $@ -b boot/kernelcore.img ${IMG_DIR}


clean:
	rm ${BUILDDIR}/*.o
	rm ${BUILDDIR}/*.elf
	rm ${BUILDDIR}/*.bin
	rm ${BUILDDIR}/ -rf image