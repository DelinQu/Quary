

## 编译原理实验2说明文档 

​																																																				——created by 屈德林 in 2020/11/14

目录：

[TOC]



## 简介

- 本文件是对lab2目录下的NFA确定化和DFA最小化代码的介绍，你将会了解lab2目录结构，以及程序运行的方法。



## 运行环境

- 操作系统：Ubuntu20.04 LTS（Linux环境）
- 操作软件：VsCode



## 文件目录

```bash
$ lab
.
├── deterNFA.cpp
├── global.cpp
├── global.h
├── main.cpp
├── Makefile
├── minDFA.cpp
└── sample
    ├── sample1.py
    ├── sample2.py
    └── sample3.py
```

- `deterNFA.cpp`：NFA确定化代码
- `global.cpp`：全局头文件实现，定义了图结构全局类型变量
- `main.c`：入口文件
- `Makefile`：Makefile规则
- `global.h`：全局头文件实现，生命所有的全局变量和函数
- minDFA：DFA最小化
- `samples`：三个测试样例



## 如何运行？

- 键入命令`make`进行编译：

```bash
$ make
g++  -c main.cpp
g++  -c deterNFA.cpp
g++  -c minDFA.cpp
g++  -c global.cpp
g++  main.o deterNFA.o minDFA.o global.o -o main

-------------------------------------------
Hi Coder! You compiled successfully!
-------------------------------------------
```

- 键入命令 `$ ./main samples/sample*.py `对样例进行测试：

```bash
$ ./main sample/sample1.py 
$ ./main sample/sample2.py 
$ ./main sample/sample3.py 
```

- `make clean`清理：

```bash
$ make clean
rm main.o deterNFA.o minDFA.o global.o
rm main

-------------------------------------------
Hi Coder! You clean successfully!
-------------------------------------------
```