# OpenCandle 开源项目

### 简介
OpenCandle由 一点不爱汇编 建立的一个基于GPL-3.0开源协议的操作系统，于2024年11月1日建立，OpenCandle欢迎任何人使用与修改，且将持续更新和维护。

### 编译条件
1. **系统环境**：Linux(推荐：Ubuntu/Debian)
2. **工具配置**：需安装qemu、make、gcc、nasm、virtaulbox(编译VHD虚拟硬盘必装)

### 编译方式
1. 获取本仓库源码到本地
2. 进入含有**Makefile**文件的目录下
3. 编译命令：
    1. 输入"make build_vhd Qrun_vhd"编译运行vhd镜像
    2. 输入"make build_img Qrun_img"编译运行img镜像
4. 其他指令：
    2. 输入"make clvhd"清空vhd相关文件
    1. 输入"make climg"清空img相关文件

### 关于 fvdisk 工具
1. 此工具适用 FAT16 文件系统
2. 此工具目前还未完善整理，只提供简单的基础功能

### 版权声明
OpenCandle操作系统项目与源码的所有者为 (QQ)一点不爱汇编，并且保留最终解释权。