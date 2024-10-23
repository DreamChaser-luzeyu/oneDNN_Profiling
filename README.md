# Usage
```bash
IN_FILE="$(pwd)/input/default.in" make matmul_nosve
IN_FILE="$(pwd)/input/default.in" make matmul_sve
make test_naive_default
make test_naive_nosve
make test_naive_sve
```
For silent monochrome test
```bash
# 2D Matmul
IN_FILE="$(pwd)/input/matmul.in" taskset -c 1 make matmul_nosve | grep Calculation
IN_FILE="$(pwd)/input/matmul.in" taskset -c 1 make matmul_sve | grep Calculation

# Batch matmul 128 128*128 128*128 matmul 
IN_FILE="$(pwd)/input/batchmm.in" taskset -c 1 make batchmm_nosve | grep Calculation
IN_FILE="$(pwd)/input/batchmm.in" taskset -c 1 make batchmm_asimd | grep Calculation
IN_FILE="$(pwd)/input/batchmm.in" taskset -c 1 make batchmm_sve | grep Calculation
# Batch matmul 110 110*110 110*110 matmul 
IN_FILE="$(pwd)/input/batchmm_2.in" taskset -c 1 make batchmm_nosve | grep Calculation
IN_FILE="$(pwd)/input/batchmm_2.in" taskset -c 1 make batchmm_asimd | grep Calculation
IN_FILE="$(pwd)/input/batchmm_2.in" taskset -c 1 make batchmm_sve | grep Calculation

# Batch matsum batch 128 channel 128 height 1024 width 1024
IN_FILE="$(pwd)/input/matsum.in" taskset -c 1 make matsum_nosve | grep Calculation
IN_FILE="$(pwd)/input/matsum.in" taskset -c 1 make matsum_asimd | grep Calculation
IN_FILE="$(pwd)/input/matsum.in" taskset -c 1 make matsum_sve | grep Calculation

# Softmax
IN_FILE="$(pwd)/input/softmax.in" taskset -c 1 make softmax_nosve | grep Calculation
IN_FILE="$(pwd)/input/softmax.in" taskset -c 1 make softmax_sve | grep Calculation
IN_FILE="$(pwd)/input/relu.in" taskset -c 1 make relu_nosve | grep Calculation
IN_FILE="$(pwd)/input/relu.in" taskset -c 1 make relu_sve | grep Calculation
```
