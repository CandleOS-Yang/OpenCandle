vbeModeNums_v: dw 0x0000; VBE可用模式号数量
vbeBpp: db 0x00; VBE每像素位数

VBE_MODE_NUM_c    equ 0xf000; VBE模式号
VBE_VERSION_c     equ 0xf002; VBE版本号
VBE_BITS_PER_PX_c equ 0xf004; VBE每像素位数
VBE_X_RES_c       equ 0xf006; VBE X分辨率
VBE_Y_RES_c       equ 0xf008; VBE Y分辨率
VBE_VRAM_c        equ 0xf00a; VBE 帧缓冲区地址

setVbeModesErr_s:; 设置VBE模式失败信息
    db "[ ERROR ] Seted the VBE Modes Error!",0


saveVbeMode_f:
; fs:bx=模式号存储内存地址
; 获取vbe信息块
    mov di,0x3000
    mov ax,0x4f00
    int 0x10

    cmp ax,0x004f
    jne .saveErr

    ; 判断版本号
    cmp word [es:di + 0x04],0x0200
    jb .saveErr
    mov ax,[es:di + 0x04]
    mov [VBE_VERSION_c],ax; 存储版本号

    ; 保存VBE模式列表
    mov gs,[es:di + 0x0e + 2]
    mov si,[es:di + 0x0e]
    mov di,0x3000 + 256
    .saveLoop:
        mov ax,0x4f01
        int 0x10

        cmp ax,0x004f
        jne .nxtMode

        cmp byte [vbeBpp],8
        je .if_bpp8
        cmp byte [vbeBpp],32
        je .if_bpp32
        jmp .saveErr

        .if_bpp8:
            cmp byte [es:di + 25],8
            je .save
            jmp .nxtMode

        .if_bpp32:
            cmp byte [es:di + 25],32
            je .save
            jmp .nxtMode

        .save:
            mov dx,[gs:si]; 模式号
            mov [fs:bx],dx
            mov al,[es:di + 25]; 每像素位数
            mov [fs:bx + 2],al
            mov ax,[es:di + 18]; 分辨率信息
            mov [fs:bx + 3],ax
            mov ax,[es:di + 20]
            mov [fs:bx + 5],ax
            mov eax,[es:di + 40]; 帧缓冲区地址
            mov [fs:bx + 7],eax
            add bx,11

    .nxtMode:
        add si,2
        mov cx,[gs:si]
        cmp cx,0xffff; 模式列表魔数
        jne .saveLoop

    .saveSuccess:
        sub bx,11
        mov [vbeModeNums_v],bx
        ret

    .saveErr:
        mov si,setVbeModesErr_s
        mov dx,0x0000
        mov bl,0x0c
        call print_f
        ret