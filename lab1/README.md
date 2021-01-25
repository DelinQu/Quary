

 ![Quary4](README.assets/Quary4.png)

## 编译原理实验1说明文档

​																																																				——created by 屈德林 in 2020/11/10

目录：

[TOC]

## 简介

- 本文件是对Quary目录下的Quary词法分析器代码的介绍，你将会了解src目录结构，以及程序运行的方法。



## 运行环境

- 操作系统：Ubuntu20.04 LTS
- 操作软件：VsCode



## 文件目录

```bash
.
├── DFA.drawio
├── globals.h
├── main.c
├── Makefile
├── samples
│   ├── sample1.qy
│   ├── sample2.qy
│   └── sample3.qy
├── scan.c
├── scan.h
├── util.c
└── util.h
```

- `DFA.draw`：DFA状态图，请使用VsCode打开它
- `globals.h`：全局头文件，定义了TINY编译器的全局类型和变量
- `main.c`：入口文件
- `Makefile`：Makefile规则
- `scan.c`：此法分析文件主体
- `utils.c`：工具类
- `scan.h`：词法分析器头文件
- `util.h`：工具类头文件

- `samples`：三个测试样例



## 如何运行？

- 键入命令`make`进行编译：

```bash
$ make
gcc -w -c main.c
gcc -w -c util.c
gcc -w -c scan.c
gcc -w main.o util.o scan.o -o Quary

-------------------------------------------
Hi Coder! You compiled Quary successfully!
=> Welcome to Quary! <=
-------------------------------------------
```

- 键入命令 `$ ./Quary samples/sample*.qy `对样例进行测试：

```bash
$ ./Quary samples/sample1.qy 
$ ./Quary samples/sample2.qy 
$ ./Quary samples/sample3.qy 
```

- `make clean`清理：

```bash
$ make clean
rm main.o util.o scan.o
rm Quary

-------------------------------------------
Hi Coder! You clean Quary successfully!
-------------------------------------------

```

