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

${BUILDDIR}/%.elf: ${BUILDDIR}/%.o
	${LD} ${KERNEL_LDFLAGS} -Ttext 0 $< -o $@

${BUILDDIR}/%.bin: ${BUILDDIR}/%.elf
	${OBJCOPY} -O binary $< $@

${BUILDDIR}/kernelcore.img: ${BUILDDIR}/bootloader.bin ${BUILDDIR}/kernelboot.bin
	cat $^ /dev/zero | head -c 1474560 > $@
	
${BUILDDIR}/bootloader.iso: ${BUILDDIR}/kernelcore.img
	rm -rf ${IMG_DIR}
	mkdir ${IMG_DIR} ${IMG_DIR}/boot
	mv ${BUILDDIR}/kernelcore.img ${IMG_DIR}/boot
	${ISOGEN} -input-charset utf-8 -iso-level 3 -J -R -o $@ -b boot/kernelcore.img ${IMG_DIR}


clean:
	rm ${BUILDDIR}/*.bin
	rm -rf ${BUILDDIR}/image