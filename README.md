# Simple brainfuck interpreter

- Written in C language.
- Only 100 lines were used to implement.
- No address size limit.
- Quick backtracking.
- Comprehensive error checking.

## Usage

```shell
$ cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
$ cmake --build build --config Release
$ ./build/brainfuck ./testsuite/helloworld.bf
Hello World!
```
