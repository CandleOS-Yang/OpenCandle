org 0x1000

dw 0xCDE1

jmp start_b

%include './boot/include/print.inc'
%include './boot/include/disk.inc'
%include './boot/include/mem.inc'
%include './boot/include/menu.inc'
%include './boot/include/loader.inc'
%include './boot/include/vbe.inc'

PAGE_DIR_TABLE_POS equ 0x100000
PG_P  equ   1b
PG_RW_R	 equ  00b 
PG_RW_W	 equ  10b 
PG_US_S	 equ  000b 
PG_US_U	 equ  100b 

start_b:
    ; 初始化段寄存器
    mov ax,0
    mov ds,ax
    mov es,ax
    mov ss,ax
    mov fs,ax
    mov gs,ax
    mov sp,0x7c00

    mov edi,ardsBufAddr_v + 4
    call memTest_f
    mov edi,ardsBufAddr_v
    mov eax,[ardsCnt_v]
    mov [es:edi],eax

    ; 清除屏幕信息
    mov ax,0x0003
    int 0x10

    mov si,menuWel_s
    mov bl,0x0e
    mov dx,0x0600 + 40 - (22 / 2)
    call print_f
    mov dx,0x0700 + 40 - (24 / 2)
    call print_f
    
    mov dx,0x0d2e
    call setCur_f

menu_loop_b:
    call dispMenu_f
    call input_f
    call dispMenuChse_f
    jmp menu_loop_b


prepare_protected_mode_b:
    ;关闭中断
    cli

    ;设置A20
    in al,0x92
    or al,0000_0010b
    out 0x92,al

    ;加载GDT
    lgdt [gdt_ptr_v]

    ;设置CR0寄存器
    mov eax,cr0
    or eax,0x00000001
    mov cr0,eax

    ;跳转刷新指令流水线
    jmp dword code_selector_c:protected_mode_b

[bits 32]
protected_mode_b:
    mov ax,data_selector_c
    mov ds,ax
    mov es,ax
    mov ss,ax
    mov fs,ax
    mov gs,ax
    mov esp,0x10000

    ; 开启SSE指令集
    mov eax,cr0
    and ax,0xFFFB; 禁用协处理器仿真模式并启用协处理器监控模式
    or ax,0x2
    mov cr0,eax
    mov eax,cr4
    or eax,0x600
    mov cr4,eax

    ; 迁移内核到0x8000以上
    xor ecx,ecx
    mov cl,[fileSecNum_v]
    mov esi,0x10000
    mov edi,0x8000
    .memcpy_kernel:
        mov dx,512 / 16
        .memcpy_loops:
            movups xmm0,[edi]
            movups [esi],xmm0
            add edi,16
            add esi,16
            dec dx
            cmp dx,0
            jne .memcpy_loops
        loop .memcpy_kernel

    jmp dword code_selector_c:0x10000

times (4 * 512 - 2) - ($ - $$) db 0
dw 0xCDE1