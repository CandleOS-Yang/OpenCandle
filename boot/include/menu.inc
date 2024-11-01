menuChseNum_v: dw 1; 当前选择项标识

menuWel_s:
    db "Welcome to CandleOS-X!",0
    db "Please choose an option!",0
menuItems_s:
    db "[ OPTION ] Candle OS-X          ",0
    db "[ OPTION ] Candle OS-X(8bit)    ",0
    db "[ OPTION ] Reboot your computer ",0
    db "[ OPTION ] Power off            ",0
menuChse_s:
    db "        Your choose: [",0
    db "]        ",0

dispMenu_f:
    mov di,160 * 9 + (40 - (32 / 2)) * 2
    mov cx,1
    mov si,menuItems_s
    .menuLoop:
        push di
        cmp cx,[menuChseNum_v]
        je .highLight
        mov bl,0x0f
        call printm_f
        pop di
        jmp .nxtItem
        .highLight:
            mov bl,0x4f
            call printm_f
            pop di
        .nxtItem:
            inc si
            add di,160
            inc cx
            cmp cx,5
            jne .menuLoop
    ret

dispMenuChse_f:
    mov bl,0x0f
    mov si,menuChse_s
    mov di,160 * 0x0d + (40 - (32 / 2)) * 2
    call printm_f
    inc si
    add di,2
    call printm_f

    mov ah,0x09
    mov al,[menuChseNum_v]
    add al,0x30
    mov cx,1
    int 0x10
    ret

input_f:
    ;检查键盘缓冲区
    mov ah,0x01
    int 0x16
    jz .end
    ;获取键盘输入
    mov ah,0x00
    int 0x16

    ;检查输入项
    cmp al,'1'
    je .chse1
    cmp al,'2'
    je .chse2
    cmp al,'3'
    je .chse3
    cmp al,'4'
    je .chse4
    cmp al,0x0d
    je .chseEnter
    cmp ah,0x48
    je .chseUP
    cmp ah,0x50
    je .chseDN
    ret
        
    ;事件处理
    .chseEnter:
        call enter_chse_f
        ret
    .chse1:
        mov word [menuChseNum_v],1
        ret
    .chse2:
        mov word [menuChseNum_v],2
        ret
    .chse3:
        mov word [menuChseNum_v],3
        ret
    .chse4:
        mov word [menuChseNum_v],4
        ret
    .chseUP:
        cmp word [menuChseNum_v],1
        je .top
        sub word [menuChseNum_v],1
        ret
        .top:
            mov word [menuChseNum_v],4
            ret
    .chseDN:
        cmp word [menuChseNum_v],4
        je .bottom
        add word [menuChseNum_v],1
        ret
        .bottom:
            mov word [menuChseNum_v],1
    .end:
        ret

enter_chse_f:
    cmp word [menuChseNum_v],1
    je .enter_chse1
    cmp word [menuChseNum_v],2
    je .enter_chse2
    cmp word [menuChseNum_v],3
    je .enter_chse3
    cmp word [menuChseNum_v],4
    je .enter_chse4

    .enter_chse1:
        mov bx,0x7000
        mov byte [vbeBpp],32
        call saveVbeMode_f
        xor eax,eax
        mov es,ax
        mov di,[vbeModeNums_v]

        ;保存VBE模式信息
        mov ax,[es:di]
        mov [VBE_MODE_NUM_c],ax
        mov al,[es:di + 2]
        mov [VBE_BITS_PER_PX_c],al
        mov ax,[es:di + 3]
        mov [VBE_X_RES_c],ax
        mov ax,[es:di + 5]
        mov [VBE_Y_RES_c],ax
        mov eax,[es:di + 7]
        mov [VBE_VRAM_c],eax

        ;设置VBE模式
        mov ax,0x4f02
        mov bx,[es:di]
        add bx,0x4000 ;使用线性帧缓存区模式
        int 0x10

        cmp ax,0x004f
        jne saveVbeMode_f.saveErr

        call readKernelFile_f
        jmp prepare_protected_mode_b
        ret

    .enter_chse2:
        mov bx,0x7000
        mov byte [vbeBpp],8
        call saveVbeMode_f
        xor eax,eax
        mov es,ax
        mov di,[vbeModeNums_v]

        ;保存VBE模式信息
        mov ax,[es:di]
        mov [VBE_MODE_NUM_c],ax
        mov al,[es:di + 2]
        mov [VBE_BITS_PER_PX_c],al
        mov ax,[es:di + 3]
        mov [VBE_X_RES_c],ax
        mov ax,[es:di + 5]
        mov [VBE_Y_RES_c],ax
        mov eax,[es:di + 7]
        mov [VBE_VRAM_c],eax

        ;设置VBE模式
        mov ax,0x4f02
        mov bx,[es:di]
        add bx,0x4000 ;使用线性帧缓存区模式
        int 0x10

        cmp ax,0x004f
        jne saveVbeMode_f.saveErr

        call readKernelFile_f
        jmp prepare_protected_mode_b
        ret

    .enter_chse3:
        ;重启
        in ax,0x92
        or ax,0x0001
        out 0x92,ax
        ret
    .enter_chse4:
        ;高级电源管理-关机
        mov ax,0x5301
        xor bx,bx
        int 0x15
        mov ax,0x530e
        mov cx,0x0102
        int 0x15
        mov ax,0x5307
        mov bx,0x0001
        mov cx,0x0003
        int 0x15
        ret
