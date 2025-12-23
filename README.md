# hill cipher with assembly

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
