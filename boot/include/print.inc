print_f:
;si=字符串
;bl=属性
;dh = 行
;dl = 列
    push cx
    mov cx,1
    .printLoop:
        call setCur_f

        mov al,[si]; 获取字符
        cmp al,0
        je .end; 判断结束

        mov ah,0x09
        int 0x10

        inc si
        inc dl
        jmp .printLoop
    .end:
        inc si
        pop cx
        ret

printm_f:
;si=字符串
;bl=属性
;di=位置
    mov ax,0xb800
    mov es,ax
    .printLoop:
        mov al,[si]
        cmp al,0
        je .end

        mov byte [es:di],al
        mov byte [es:di + 1],bl
        inc si
        add di,2 
        jmp .printLoop
    .end:
        ret

setCur_f:
;dh = 行
;dl = 列
    mov ah,0x02
    mov bh,0x00
    int 0x10
    ret