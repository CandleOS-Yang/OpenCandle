fileSecNum_v: db 0x00; 文件扇区数
kernelFileName_v: db "KERNEL  BIN"; 内核文件名称

KERNEL_FILE_LEN_c equ  11; 内核文件长度
DIR_FSTCLUS_c     equ  26; 文件起始簇号偏移

readKernelFile_f:
    ;读取根目录区
    mov cx,0x0001; 读取扇区数
    mov esi,0x7e00; 缓冲区地址
    mov ebx,69; 根目录区起始扇区号
    call readDisk_f

    ;判断文件名称
    mov si,0x7e00
    mov di,kernelFileName_v
    mov cx,KERNEL_FILE_LEN_c; 11字节
    mov dx,0
    
    .checkFileName:
        shl dx,5
        add si,dx
        repe cmpsb; 循环比较名称
        jcxz .foundFile
        inc dx
        cmp dx,32; 32个目录项
        jb .checkFileName
    .end:
        ret

    .foundFile:
        sub si,KERNEL_FILE_LEN_c; 回到目录项起始地址
        add si,DIR_FSTCLUS_c
        xor edx,edx
        mov dx,[si];获取文件起始簇号

        ;读取FAT表
        mov cx,0x0001; 读取扇区数
        mov esi,0x7e00; 缓冲区地址
        mov ebx,5; FAT表起始扇区号
        call readDisk_f

        mov esi,0x8000
        mov di,0x7e00
        mov ax,dx
        shl ax,1
        add di,ax
        
        .readFile:
            sub dx,2
            add dx,101

            ;读取文件数据
            mov cx,0x0001; 读取扇区数
            mov ebx,edx
            call readDisk_f
            
            mov dx,[di]

            add esi,512
            add di,2
            add word [fileSecNum_v],1
            cmp dx,0xfff8
            jb .readFile
        ret


code_selector_c equ (1 << 3)
data_selector_c equ (2 << 3)

gdt_ptr_v:
    ;GDT界限
    dw (gdt_end_b - gdt_base_v) - 1
    ;GDT基地址
    dd gdt_base_v

gdt_base_v:
    dd 0x0, 0x0
gdt_code_v:
    ;段界限0~15位
    dw 0xffff
    ;段基址0~15位
    dw 0x0000
    ;段基址16~23位
    db 0x00
    ;段属性
    db 1_00_1_1010b
    ;段界限16~19位与段属性
    db 1_1_0_0_0000b | 0xf
    ;段基址24~31位
    db 0x00

gdt_data_v:
    ;段界限0~15位
    dw 0xffff
    ;段基址0~15位
    dw 0x0000
    ;段基址16~23位
    db 0x00
    ;段属性
    db 1_00_1_0010b
    ;段界限16~19位与段属性
    db 1_1_0_0_0000b | 0xf
    ;段基址24~31位
    db 0x00

gdt_end_b:
