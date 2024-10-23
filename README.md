# Usage
```bash
IN_FILE="$(pwd)/input/default.in" make matmul_nosve
IN_FILE="$(pwd)/input/default.in" make matmul_sve
make test_naive_default
make test_naive_nosve
make test_naive_sve
```
For monocore test
```bash
IN_FILE="$(pwd)/input/matmul.in" taskset -c 1 make matmul_nosve
IN_FILE="$(pwd)/input/matmul.in" taskset -c 1 make matmul_sve
IN_FILE="$(pwd)/input/softmax.in" taskset -c 1 make softmax_nosve
IN_FILE="$(pwd)/input/softmax.in" taskset -c 1 make softmax_sve
IN_FILE="$(pwd)/input/relu.in" taskset -c 1 make relu_nosve
IN_FILE="$(pwd)/input/relu.in" taskset -c 1 make relu_sve
```
For silent test
```bash
# 2D Matmul
IN_FILE="$(pwd)/input/matmul.in" taskset -c 1 make matmul_nosve | grep Calculation
IN_FILE="$(pwd)/input/matmul.in" taskset -c 1 make matmul_sve | grep Calculation
# Batch matmul
IN_FILE="$(pwd)/input/batchmm.in" taskset -c 1 make batchmm_nosve | grep Calculation
IN_FILE="$(pwd)/input/batchmm.in" taskset -c 1 make batchmm_asimd | grep Calculation
IN_FILE="$(pwd)/input/batchmm.in" taskset -c 1 make batchmm_sve | grep Calculation
# Softmax
IN_FILE="$(pwd)/input/softmax.in" taskset -c 1 make softmax_nosve | grep Calculation
IN_FILE="$(pwd)/input/softmax.in" taskset -c 1 make softmax_sve | grep Calculation
IN_FILE="$(pwd)/input/relu.in" taskset -c 1 make relu_nosve | grep Calculation
IN_FILE="$(pwd)/input/relu.in" taskset -c 1 make relu_sve | grep Calculation
```
