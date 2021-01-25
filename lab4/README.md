

## 编译原理实验4说明文档

​																																																				——created by 屈德林 in 2020/12/13

目录：

[TOC]

## 简介

- 本文件是对src目录下**编译器自动生成工具实验**代码的介绍，你将会了解src目录结构，以及程序运行的方法。



## 运行环境

- 操作系统：Ubuntu20.04 LTS
- 操作软件：VsCode



## 文件目录

```bash
src/
├── flex
│   ├── CMinus
│   │   ├── cminus.l
│   │   ├── globals.h
│   │   ├── lex.yy.c
│   │   ├── main.c
│   │   ├── Makefile
│   │   ├── samples
│   │   │   ├── sample1.c
│   │   │   ├── sample2.c
│   │   │   └── sample3.c
│   │   ├── scan.h
│   │   ├── util.c
│   │   └── util.h
│   ├── samples
│   │   ├── Makefile
│   │   ├── sample1.L
│   │   ├── sample2.L
│   │   ├── sample3.L
│   │   └── sample4.L
│   └── tiny
│       ├── globals.h
│       ├── lex.yy.c
│       ├── main.c
│       ├── Makefile
│       ├── samples
│       │   └── sample.tny
│       ├── scan.h
│       ├── tiny.l
│       ├── util.c
│       └── util.h
├── README.md
└── yacc
    ├── Cminus
    │   ├── analyze.c
    │   ├── analyze.h
    │   ├── cminus.l
    │   ├── cminus.y
    │   ├── globals.h
    │   ├── main.c
    │   ├── Makefile
    │   ├── parse.c
    │   ├── parse.h
    │   ├── samples
    │   │   ├── error.c
    │   │   ├── errorfreea.c
    │   │   ├── errorfree.c
    │   │   ├── given_test.c
    │   │   ├── sample1.c
    │   │   ├── sample2.c
    │   │   ├── sample3.c
    │   │   └── sample_while.c
    │   ├── scan.h
    │   ├── symtab.c
    │   ├── symtab.h
    │   ├── util.c
    │   └── util.h
    ├── samples
    │   ├── calculator.Y
    │   └── Makefile
    └── tiny
        ├── globals.h
        ├── lex.yy.c
        ├── main.c
        ├── Makefile
        ├── parse.h
        ├── sample.tny
        ├── scan.h
        ├── tiny.tab.c
        ├── tiny.tab.h
        ├── tiny.y
        ├── util.c
        └── util.h
```

- `flex`: 词法分析部分
- `yacc`: 语法分析部分
- `samples`: 样例测试部分
- `tiny`: tiny的语法分析程序或词法分析程序
- `Cminus`: Cminus的此法分析程序或语法分析程序
- `README.md`: 说明文档
- `Makeflie`: make文件



## 如何运行？

- 键入命令`make`进行编译：

  仔细观察, flex和yacc目录具有高度的对称性, 这是因为我严格遵从实验报告要求. 现在你可以在任何一个子目录下执行make指令, 生成你想要的目标可执行程序

  下面是一个例子, 我在`src/flex/CMinus`目录下执行make, 将会得到一个Cminus的词法分析程序, 其他目录以此类推.

```bash
src/flex/CMinus$ ls
cminus.l  globals.h  lex.yy.c  main.c  Makefile  samples  scan.h  util.c  util.h
src/flex/CMinus$ make
gcc main.c -c -w
flex cminus.l
gcc lex.yy.c -c -w
gcc util.c -c -w
gcc main.o lex.yy.o util.o -o scanner -lfl
src/flex/CMinus$ ls
cminus.l  globals.h  lex.yy.c  lex.yy.o  main.c  main.o  Makefile  samples  scan.h  scanner  util.c  util.h  util.o
```

- 键入命令 `./scanner samples/sample1.c `对样例进行测试：

```bash
$ ./scanner samples/sample1.c 

TINY COMPILATION: samples/sample1.c
        1: reserved word: int
        1: ID, name= i
        1: ;
        2: reserved word: int
        2: ID, name= j
        2: ;
        4: reserved word: int
        4: ID, name= gcd
        4: (
        4: reserved word: int
        4: ID, name= u
        4: ,
        4: reserved word: int
.....................................
```

- `make clean`清理：

```bash
$ make clean
rm main.o
rm lex.yy.o
rm util.o
rm scanner
```

- 注意, 上述执行过程之针对在`src/flex/CMinus`目录下的程序, 如果想要测试实验中的其他程序, 请在其他目录下make并执行