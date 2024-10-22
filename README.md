# Usage
```bash
IN_FILE="$(pwd)/input/default.in" make matmul_nosve
IN_FILE="$(pwd)/input/default.in" make matmul_sve
```
For monocore test
```bash
IN_FILE="$(pwd)/input/matmul.in" taskset -c 1 make matmul_nosve
IN_FILE="$(pwd)/input/matmul.in" taskset -c 1 make matmul_sve
IN_FILE="$(pwd)/input/softmax.in" taskset -c 1 make softmax_nosve
IN_FILE="$(pwd)/input/softmax.in" taskset -c 1 make softmax_sve
IN_FILE="$(pwd)/input/relu.in" taskset -c 1 make relu_nosve
IN_FILE="$(pwd)/input/relu.in" taskset -c 1 make relu_sve
make test_naive_default
make test_naive_nosve
make test_naive_sve
```