org 0x7c00

jmp start_b

%include './boot/include/fat16.inc'
%include './boot/include/print.inc'
%include './boot/include/boot.inc'
%include './boot/include/disk.inc'

start_b:
    ; 初始化段寄存器
    mov ax,0
    mov ds,ax
    mov es,ax
    mov ss,ax
    mov sp,0x7c00

    mov [0x7dfe],dl; 存储驱动器号

    ; 清除屏幕信息
    mov ax,0x0003
    int 0x10

    mov si,mbrLoad_s
    mov bl,0x0e
    mov dx,0x0000
    call print_f

    mov si,mbrLoadSuccess_s
    mov dx,0x0100
    call print_f

    mov ecx,0x0004; 读取扇区数
    mov esi,0x00001000; 缓冲区地址
    mov ebx,1; 起始扇区号
    call readDisk_f

    cmp word [0x1000],0xCDE1; Loader魔数
    jne loader_load_err_b

    mov si,loaderLoadSuccess_s
    mov bl,0x0e
    mov dx,0x0200
    call print_f
    jmp 0:0x1002

loader_load_err_b:
    ;Loader加载失败
    mov si,loaderLoadErr_s
    mov dx,0x0200
    mov bl,0x0c
    call print_f

    hlt
    jmp $

times 510 - ($ - $$) db 0
dw 0xAA55