BS_OEMName      db "COS-X   "   ; OEM名称
BPB_BytesPerSec dw 0x0200       ; 每个扇区的字节数
BPB_SecPerClus  db 0x01         ; 每个簇扇区数
BPB_RsvdSecCnt  dw 0x0004       ; 保留扇区数
BPB_NumFATs     db 0x02         ; FAT表份数
BPB_RootEntCnt  dw 0x0200       ; 根目录项数
BPB_TotSec16    dw 0xffbb       ; 16位总扇区数
BPB_Media       db 0xf8         ; 媒体描述符
BPB_FATSz16     dw 0x0020       ; FAT表扇区数
BPB_SecPerTrk   dw 0x0020       ; 每磁道扇区数
BPB_NumHeads    dw 0x0004       ; 磁头数
BPB_HiddSec     dd 0x00000000   ; 隐藏扇区数
BPB_TotSec32    dd 0x00000000   ; 32位总扇区数
BS_DrvNum       db 0x80         ; 驱动器号
BS_Reserved1    db 0x00         ; 保留
BS_BootSig      db 0x29         ; 引导标志
BS_VolID        dd 0x00000000   ; 卷序列号
BS_VolLab       db "CandleOS-X "; 卷标
BS_FileSysType  db "FAT16   "   ; 文件系统类型