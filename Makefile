#路径
bt := ./boot
bd := ./build
# bs := ../bochsrc
kl := ./kernel
inc := ./include

drv := $(kl)/drivers
res := $(kl)/res
tools := $(kl)/tools

fonts := $(res)/fonts
pic := $(res)/pic

image := $(bd)/image

# 文件
bootfile := $(bd)/boot.bin \
			$(bd)/loader.bin

kernelfile := $(bd)/kernel.bin

kernel_objs := $(bd)/main.o \
			   $(fonts)/SongFont.obj

vhd := $(image)/CandleOS-X.vhd
img := $(image)/CandleOS-X.img

# 工具文件
fd := $(tools)/fvdisk/fvdisk.out
vboxmanage := ./../../../../usr/bin/vboxmanage

# 编译 链接选项
kernel_base := 0x10000

CFLAGS := -m32
CFLAGS += -fno-builtin
CFLAGS += -nostdinc
CFLAGS += -fno-pic
CFLAGS += -fno-pie
CFLAGS += -nostdlib
CFLAGS += -fno-stack-protector
CFLAGS += -g
CFLAGS += -I$(inc)
CFLAGS += -msse3
CFLAGS += -masm=intel


# 编译引导文件
$(bd)/%.bin: $(bt)/%.asm
	nasm -f bin $< -o $@

# 编译内核文件
$(bd)/%.o: $(kl)/main/%.c
	gcc $(CFLAGS) -c $< -o $@

# 链接内核文件
$(bd)/ELFkernel.bin: $(kernel_objs)
	ld -m elf_i386 $^ -o $@ -Ttext $(kernel_base)

$(bd)/kernel.bin: $(bd)/ELFkernel.bin
	objcopy -O binary $^ $@

# 编译工具文件
make_fd:
	gcc $(tools)/fvdisk/fvdisk.c -o $(fd)

# 生成镜像文件
make_vhd:
	$(vboxmanage) createhd --filename $(vhd) --size 32 --format VHD --variant Fixed

make_img:
	$(fd) -i $(img) -n 65536

# 写入镜像文件
write_vhd: $(bootfile) $(kernelfile)
	$(fd) $(bd)/boot.bin -o $(vhd) -s 0
	$(fd) $(bd)/loader.bin -o $(vhd) -s 1
	$(fd) $(kernelfile) -o $(vhd) -f 0

write_img: $(bootfile) $(kernelfile)
	$(fd) $(bd)/boot.bin -o $(img) -s 0
	$(fd) $(bd)/loader.bin -o $(img) -s 1
	$(fd) $(kernelfile) -o $(img) -f 0

# 清除文件
clvhd:
	rm -rf $(bd)/*.bin
	rm -rf $(bd)/*.o
	$(vboxmanage) closemedium disk $(vhd)
	rm -rf $(image)/*.vhd
	rm -rf $(tools)/fvdisk/fvdisk.exe

climg:
	rm -rf $(bd)/*.bin
	rm -rf $(bd)/*.o
	rm -rf $(image)/*.img
	rm -rf $(tools)/fvdisk/fvdisk.exe

# 构建镜像文件
build_vhd: clvhd make_fd make_vhd write_vhd
build_img: climg make_fd make_img write_img

# 运行镜像文件
Qrun_vhd:
	qemu-system-i386 -hda $(vhd) -m 128

Qrun_img:
	qemu-system-i386 -hda $(img) -m 128

# rb_vhd: $(VHD)
# 	bochsdbg -q -f $(BS)/win_vhd_bochsrc.bxrc