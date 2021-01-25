

## 编译原理实验6说明文档

​																																																				——created by 屈德林 in 2021/1/9

目录：

[TOC]

## 简介

- 本文件是对src目录下**实验六 中间代码生成器**代码的介绍，你将会了解src目录结构，以及程序运行的方法。



## 运行环境

- 操作系统：Ubuntu20.04 LTS
- 操作软件：VsCode



## 文件目录

```bash
$ tree
.
├── Cminus
│   ├── analyze.c
│   ├── analyze.h
│   ├── analyze.o
│   ├── cgen.c
│   ├── cgen.h
│   ├── cgen.o
│   ├── Cmachine
│   ├── cminus
│   ├── cminus.l
│   ├── cminus.y
│   ├── code.c
│   ├── code.h
│   ├── code.o
│   ├── globals.h
│   ├── lex.yy.c
│   ├── lex.yy.o
│   ├── main.c
│   ├── main.o
│   ├── Makefile
│   ├── parse.c
│   ├── parse.h
│   ├── samples
│   │   ├── error.c
│   │   ├── sample1.c
│   │   ├── sample1.tm
│   │   └── sample2.c
│   ├── scan.c
│   ├── scan.h
│   ├── symtab.c
│   ├── symtab.h
│   ├── symtab.o
│   ├── tm.c
│   ├── util.c
│   ├── util.h
│   ├── util.o
│   ├── y.tab.c
│   ├── y.tab.h
│   └── y.tab.o
├── README.md
└── Tiny
    ├── analyze.c
    ├── analyze.h
    ├── cgen.c
    ├── cgen.h
    ├── code.c
    ├── code.h
    ├── globals.h
    ├── main.c
    ├── Makefile
    ├── parse.c
    ├── parse.h
    ├── samples
    │   ├── Euclid.tm
    │   ├── Euclid.tny
    │   ├── sample0.tm
    │   ├── sample1.tm
    │   └── sample1.tny
    ├── scan.c
    ├── scan.h
    ├── symtab.c
    ├── symtab.h
    ├── tiny.l
    ├── tiny.y
    ├── tm.c
    ├── util.c
    └── util.h
```

- `samples`: 样例测试部分
- `Tiny`: tiny的语法分析程序或词法分析程序
- `Cminus`: Cminus的此法分析程序或语法分析程序
- `README.md`: 说明文档
- `Makeflie`: make文件



## 如何运行？

- 键入命令`make`进行编译：

  现在你可以在任何一个子目录下执行make指令, 生成你想要的目标可执行程序下面是一个例子, 我在`src/CMinus`目录下执行make, 将会得到一个Cminus的myComplier程序, 其他目录以此类推.


```bash
$ make
bison -d cminus.y --yacc
cminus.y: 警告: 1 项偏移/归约冲突 [-Wconflicts-sr]
gcc -w -g -c y.tab.c
flex -o lex.yy.c cminus.l
gcc -w -g -c lex.yy.c
gcc -w -g -c main.c
gcc -w -g -c util.c
gcc -w -g -c symtab.c
gcc -w -g -c analyze.c
gcc -w -g -c code.c
gcc -w -g -c cgen.c
gcc -w -g y.tab.o lex.yy.o main.o util.o symtab.o analyze.o code.o cgen.o -o cminus
gcc -w -g tm.c -o Cmachine
```

- 键入命令 `./cminus samples/sample1.c  `对样例sample1.c  进行代码生成：

```bash
$ ./cminus samples/sample1.c 

TINY COMPILATION: samples/sample1.c

Building Symbol Table...

Symbol table:

<global scope> (nested level: 0)
Symbol Name    Sym.Type  Data Type    Line Numbers
-------------  --------  -----------  ------------
main           Function  Void            9 
input          Function  Integer        -1   12   12 
output         Function  Void           -1   13 
gcd            Function  Integer         3    6   13 

function name: gcd (nested level: 1)
Symbol Name    Sym.Type  Data Type    Line Numbers
-------------  --------  -----------  ------------
u              Variable  Integer         3    5    6    6 
v              Variable  Integer         3    5    6    6    6 

function name: main (nested level: 1)
Symbol Name    Sym.Type  Data Type    Line Numbers
-------------  --------  -----------  ------------
x              Variable  Integer        11   12   13 
y              Variable  Integer        11   12   13 
Checking Types...
```

- 键入命令 ` ./Cmachine ./samples/sample1.tm   `运行目标代码sample1.tm ：

```bash
$ ./Cmachine ./samples/sample1.tm 
```

- `make clean`清理：

```bash
$ make clean
rm main.o
rm lex.yy.o
rm util.o
rm scanner
```

- 注意, 上述执行过程之针对在`src/CMinus`目录下的程序, 如果想要测试实验中的其他程序, 请在其他目录下make并执行

