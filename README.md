# hill cipher with assembly

## prerequisites

- *aarch64* headers.
- `qemu-aarch64`
- `make`

## using `make` to compile and run

there are two codes to compile in this project. one of them uses assembly to
compute matrix multiplication for performance. other one only uses C to do that.

### compile and test the hybrid code

this command compiles the code then executes `test.sh` to test it. test script
executes the binary then prints out the hashes to compare the output files.

```terminal
make && make test
```

### compiling the only-C code

this command only compiles the code that written with full-C. it doesn't have 
any automated test. you can use any hash algorithm to compare outputs.

this side of the project is to compare the throughputs wiht hybrid one.

```terminal
make onlyc
```

## benchmarking

it's actually simple. timer starts before matrix multiplication and ends after.

$$
\text{throughput} = \frac{\text{size}}{s}
$$

the arm assembly code uses *neon* vector registers to achieve speed of paralellism.
on the other hand only-C code uses flatten c arrays and mercy of the compilers 
optimization process.

## creating test file with `dd`

### general usage

```terminal
dd if=/dev/zero of=FILE bs=BLOCK_SIZE count=COUNT
```

### 100 MB random

```terminal
dd if=/dev/random of=test.bin bs=1M count=100
```

### 10 MB random

```terminal
dd if=/dev/urandom of=test.bin bs=1M count=10
```

### 10 MB zero

```terminal
dd if=/dev/zero of=test.bin bs=1M count=10
```

## files that its size isn't divisible with block size (=4)

this code is uses constant 4x4 matrices to make the project complexity minimal.
so this hill cipher uses blocks of 4 bytes. like any block ciphers, hill cipher
adds zeros to the end to fill the last block. if you want to try this code with
real files (like images or documents) you should trim the *decrypted* file to
its original size.

### check the exact byte size of the *original* file

```terminal
wc -c testfile.data
```

lets say its output is `2045`

### trim the *decrypted* file to that size

use the number from previous step

```terminal
head -c 2045 testfile.data_decrypted.bin > testfile_trimmed.bin
```

### you can compare hashes now

```terminal
sha256sum test.bin*
```