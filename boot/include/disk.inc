DAP_t:; DAP结构体
    db 0x10
    db 0
    dw 0
    dd 0
    dq 0
    dq 0

readDisk_f:
; cx=传输扇区数
; ebx=起始扇区号
; esi=缓冲区地址(段基址:偏移)
    push dx
    push esi
    mov word [DAP_t + 0x02],cx; 传输扇区数
    mov dword [DAP_t + 0x04],esi; 缓冲区地址
    mov dword [DAP_t + 0x08],ebx; 起始扇区号
    mov si,DAP_t
    mov dl,[0x7dfe]; 驱动器号
    mov ah,0x42
    int 0x13
    pop esi
    pop dx
    ret