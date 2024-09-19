GCC=/home/mateusz/opt/cross/bin/i686-elf-gcc
LD=/home/mateusz/opt/cross/bin/i686-elf-ld
AR=/home/mateusz/opt/cross/bin/i686-elf-ar
OBJCOPY=/home/mateusz/opt/cross/bin/i686-elf-objcopy
GCC_FLAGS=-Wall -c -ffreestanding -fno-pie
ISOGEN=genisoimage
BUILDDIR=build
IMG_DIR=${BUILDDIR}/image
KERN_DEPS = ${BUILDDIR}/main.o ${BUILDDIR}/print.o ${BUILDDIR}/paging.o
DATA_SECTIONS = --remove-section=.kernel_boot
TEXT_SECTIONS = --only-section=.kernel_boot

all: ${BUILDDIR}/bootloader.iso

${BUILDDIR}/%.o: %.S
	${GCC} ${GCC_FLAGS}  $< -o $@

${BUILDDIR}/%.o: kernel/%.c
	${GCC} ${GCC_FLAGS} $< -o $@

${BUILDDIR}/bootloader.elf: ${BUILDDIR}/bootloader.o
	${LD}  -Ttext 0 $< -o $@

${BUILDDIR}/bootloader.bin: ${BUILDDIR}/bootloader.elf
	${OBJCOPY} -O binary $< $@


${BUILDDIR}/kernelboot.elf: ${BUILDDIR}/kernelboot.o ${KERN_DEPS}
	${LD}  -T setup.ld $^ -o $@

${BUILDDIR}/kernelboot.bin: ${BUILDDIR}/kernelboot.elf
	${OBJCOPY} -O binary  ${DATA_SECTIONS} $< ${BUILDDIR}/data.bin
	${OBJCOPY} -O binary  ${TEXT_SECTIONS} $< ${BUILDDIR}/text.bin
	cat ${BUILDDIR}/text.bin ${BUILDDIR}/data.bin > $@

${BUILDDIR}/kernelcore.img: ${BUILDDIR}/bootloader.bin ${BUILDDIR}/kernelboot.bin
	cat $^ /dev/zero | head -c 1474560 > $@
	
${BUILDDIR}/bootloader.iso: ${BUILDDIR}/kernelcore.img
	rm -rf ${IMG_DIR}
	mkdir ${IMG_DIR} ${IMG_DIR}/boot
	mv ${BUILDDIR}/kernelcore.img ${IMG_DIR}/boot
	${ISOGEN} -input-charset utf-8 -iso-level 3 -J -R -o $@ -b boot/kernelcore.img ${IMG_DIR}


clean:
	rm -rf ${BUILDDIR}/*.o 
	rm -rf ${BUILDDIR}/*.elf
	rm -rf ${BUILDDIR}/*.bin
	rm -rf ${BUILDDIR}/image