# Usage
```bash
IN_FILE="$(pwd)/input/default.in" make matmul_nosve
IN_FILE="$(pwd)/input/default.in" make matmul_sve
```
For monocore test
```bash
IN_FILE="$(pwd)/input/default.in" taskset -c 1 make matmul_nosve
IN_FILE="$(pwd)/input/default.in" taskset -c 1 make matmul_sve
make test_naive_default
make test_naive_nosve
make test_naive_sve
```