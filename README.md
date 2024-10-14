# Usage
```bash
IN_FILE="$(pwd)/input/default.in" make test_matmul_amd64
IN_FILE="$(pwd)/input/default.in" make test_matmul_aarch64nosve
IN_FILE="$(pwd)/input/default.in" make test_matmul_aarch64sve
IN_FILE="$(pwd)/input/default.in" make test_matmul_aarch64sve_gcc13
```