# Usage
```bash
IN_FILE="$(pwd)/input/default.in" make test_matmul_amd64
IN_FILE="$(pwd)/input/default.in" make test_matmul_aarch64nosve
IN_FILE="$(pwd)/input/default.in" make test_matmul_aarch64sve
IN_FILE="$(pwd)/input/default.in" make test_matmul_aarch64sve_gcc13
```
For monocore test
```bash
IN_FILE="$(pwd)/input/default.in" taskset -c 1 make test_matmul_amd64
IN_FILE="$(pwd)/input/default.in" taskset -c 1 make test_matmul_aarch64nosve
IN_FILE="$(pwd)/input/default.in" taskset -c 1 make test_matmul_aarch64sve
IN_FILE="$(pwd)/input/default.in" taskset -c 1 make test_matmul_aarch64sve_gcc13
make test_naive_default
make test_naive_nosve
make test_naive_sve
```