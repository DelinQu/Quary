

## 编译原理实验5说明文档

​																																																				——created by 屈德林 in 2020/12/17

目录：

[TOC]

## 简介

- 本文件是对src目录下**实验五 语义分析器**代码的介绍，你将会了解src目录结构，以及程序运行的方法。



## 运行环境

- 操作系统：Ubuntu20.04 LTS
- 操作软件：VsCode



## 文件目录

```bash
.
├── Cminus
│   ├── analyze.c
│   ├── analyze.h
│   ├── cminus.l
│   ├── cminus.y
│   ├── globals.h
│   ├── main.c
│   ├── Makefile
│   ├── parse.c
│   ├── parse.h
│   ├── samples
│   │   ├── error.c
│   │   ├── errorfreea.c
│   │   ├── errorfree.c
│   │   ├── given_test.c
│   │   ├── sample1.c
│   │   ├── sample2.c
│   │   ├── sample3.c
│   │   └── sample_while.c
│   ├── scan.h
│   ├── symtab.c
│   ├── symtab.h
│   ├── util.c
│   └── util.h
├── README.md
└── Tiny
    ├── analyze.c
    ├── analyze.h
    ├── cgen.c
    ├── cgen.h
    ├── code.c
    ├── code.h
    ├── globals.h
    ├── lex
    │   └── tiny.l
    ├── main.c
    ├── Makefile
    ├── parse.c
    ├── parse.h
    ├── readme.unx
    ├── samples
    │   ├── Euclid.tny
    │   ├── sample.tm
    │   └── sample.tny
    ├── scan.c
    ├── scan.h
    ├── symtab.c
    ├── symtab.h
    ├── tm.c
    ├── util.c
    ├── util.h
    └── yacc
        ├── globals.h
        └── tiny.y
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
bison -d -v cminus.y
cminus.y: 警告: 1 项偏移/归约冲突 [-Wconflicts-sr]
gcc -w -o main.o -c main.c
gcc -w -o util.o -c util.c
flex -w cminus.l
gcc -w -o analyze.o -c analyze.c
gcc -w -o symtab.o -c symtab.c
gcc -w -o myMinusComplier main.o util.o cminus.tab.c lex.yy.c analyze.o symtab.o  -ll
```

- 键入命令 `./myComplier samples/sample1.c `对样例进行测试：

```bash
$ ./myMinusComplier samples/sample1.c 

Building Symbol Table & Checking Types...

Scope  Variable Name Location Type isArr ArrSize isFunc isParam Line Numbers
-----  ------------- -------- ---- ----- ------- ------ ------- ------------
1      u              3        int   no   0         no   yes         4    5    6    6 
1      v              4        int   no   0         no   yes         4    5    6    6    6 
Type error at line 13: unknown function name
1      x              0        int   no   0         no   no         11   13 
1      y              1        int   no   0         no   no         11   13 
0      main           0        void  no   0         yes  no         10 
0      i              0        int   no   0         no   no          1 
0      j              1        int   no   0         no   no          2 
0      gcd            2        int   no   0         yes  no          4 
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

![image-20201227232028475](https://i.loli.net/2020/12/27/vEbtxUscHFGYCSR.png)