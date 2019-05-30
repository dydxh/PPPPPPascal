# PPPPPPascal

Compile Principle Course Project

Yet Another Pascal Compiler (yapc)

## Intro

A lite pascal compiler developed on

```
bison 3.3.2
flex 2.6.4
llvm 9.0.0
```

## Build

```bash
mkdir build
cd build
cmake ..
./yapc ../test/fib.pas
```