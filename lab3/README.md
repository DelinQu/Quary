

## 编译原理实验3说明文档

​																																																				——created by 屈德林 in 2020/12/04

目录：

[TOC]

## 简介

- 本文件是对src目录下的Quary语法分析器代码的介绍，你将会了解src目录结构，以及程序运行的方法。



## 运行环境

- 操作系统：Ubuntu20.04 LTS
- 操作软件：VsCode



## 文件目录

```bash
.
├── draw
│   ├── DFA.drawio
│   └── parse.drawio
├── globals.h
├── main.c
├── Makefile
├── parse.c
├── parse.h
├── README.md
├── samples
│   ├── sample1.py
│   ├── sample2.py
│   └── sample3.py
├── scan.c
├── scan.h
├── util.c
└── util.h
```

- `DFA.draw`：DFA状态图，请使用VsCode打开它
- `parse.drawio`：语法分析树递归图，请用VsCode打开它
- `globals.h`：全局头文件，定义了TINY编译器的全局类型和变量
- `main.c`：入口文件
- `Makefile`：Makefile规则
- `scan.c`：此法分析文件主体
- `utils.c`：工具类
- `scan.h`：词法分析器头文件
- `util.h`：工具类头文件
- `samples`：三个测试样例
- `parse.h`：语法分析头文件
- `parse.c`：语法分析文件



## 如何运行？

- 键入命令`make`进行编译：

```bash
$ make
gcc -w -c main.c
gcc -w -c util.c
gcc -w -c scan.c
gcc -c parse.c -o parse.o
gcc -w main.o util.o scan.o parse.o -o QuaryParse

-----------------------------------------------
Hi Coder! You compiled QuaryParse successfully!
=> Welcome to QuaryParse! <=
-----------------------------------------------
```

- 键入命令 `$ ./QuaryParse samples/sample*.qy `对样例进行测试：

```bash
$ ./QuaryParse samples/sample1.qy 
$ ./QuaryParse samples/sample2.qy 
$ ./QuaryParse samples/sample3.qy 
```

- `make clean`清理：

```bash
rm main.o util.o scan.o parse.o
rm QuaryParse

-----------------------------------------------
Hi Coder! You clean QuaryParse successfully!
-----------------------------------------------
```

