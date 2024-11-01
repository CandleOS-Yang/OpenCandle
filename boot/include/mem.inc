memTestSuccess_s:; 内存测试成功信息
    db "[ INFO ] Candle OS-X Memory tested successfully!",0

memTestErr_s:; 内存测试错误信息
    db "[ ERROR ] Candle OS-X Memory tested error!",0

ardsCnt_v:
    dd 0

ardsBufAddr_v equ 0x5000
    

memTest_f:
; es:di=ARDS缓冲区地址
    xor ebx,ebx; 将ebx置为 0
    mov edx,0x534d4150; 固定魔数
    .nxtTest:
        mov eax,0xe820
        mov ecx,20; ARDS 结构的字节大小为20字节
        int 0x15

        jc .err; CF置位时，表示出错

        add di,cx

        inc dword [ardsCnt_v]

        cmp ebx,0
        jnz .nxtTest

        mov si,memTestSuccess_s
        mov bl,0x0e
        mov dx,0x0400
        call print_f
        ret
    .err:
        mov si,memTestErr_s
        mov bl,0x0c
        mov dx,0x0400
        call print_f
        ret