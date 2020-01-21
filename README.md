# pl0c
## Get Started
```bash
# make compiler, vm
make
# compile cmm
./compiler/cmmc ./examples/cmm/for.cmm
# exec pl0
./vm/pl0vm a.out
```

## Environment
```
$ lex -V
flex 2.5.35 Apple(flex-31)

$ yacc -V
bison (GNU Bison) 2.3
Written by Robert Corbett and Richard Stallman.

$ gcc -v
Apple LLVM version 10.0.1 (clang-1001.0.46.4)
```

## Directory
```
├── compiler // cmm compiler src
├── examples // cmm, pl0 examples
├── test // test scripts
└── vm // pl0 vm src
```
