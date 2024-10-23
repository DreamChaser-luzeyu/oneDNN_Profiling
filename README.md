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
# Batch matsum batch 128 channel 128 height 1024 width 1024, maybe useless
# IN_FILE="$(pwd)/input/matsum.in" taskset -c 1 make matsum_nosve | grep LOG
# IN_FILE="$(pwd)/input/matsum.in" taskset -c 1 make matsum_asimd | grep LOG
# IN_FILE="$(pwd)/input/matsum.in" taskset -c 1 make matsum_sve | grep LOG

# Batch matmul 16 16*16 16*16 matmul, dump result
IN_FILE="$(pwd)/input/batchmm_3.in" taskset -c 1 make batchmm_nosve | grep LOG
# IN_FILE="$(pwd)/input/batchmm_3.in" taskset -c 1 make batchmm_asimd | grep LOG
IN_FILE="$(pwd)/input/batchmm_3.in" taskset -c 1 make batchmm_sve | grep LOG
diff ./batchmm.out.nosimd ./batchmm.out.sve
# Batch matmul 128 128*128 128*128 matmul, no dump result 
IN_FILE="$(pwd)/input/batchmm.in" taskset -c 1 make batchmm_nodump_nosve | grep LOG
# IN_FILE="$(pwd)/input/batchmm.in" taskset -c 1 make batchmm_nodump_asimd | grep LOG
IN_FILE="$(pwd)/input/batchmm.in" taskset -c 1 make batchmm_nodump_sve | grep LOG
# Batch matmul 110 110*110 110*110 matmul, no dump result
IN_FILE="$(pwd)/input/batchmm_2.in" taskset -c 1 make batchmm_nodump_nosve | grep LOG
# IN_FILE="$(pwd)/input/batchmm_2.in" taskset -c 1 make batchmm_asimd | grep LOG
IN_FILE="$(pwd)/input/batchmm_2.in" taskset -c 1 make batchmm_nodump_sve | grep LOG

# Softmax, dump result
IN_FILE="$(pwd)/input/softmax_2.in" taskset -c 1 make softmax_nosve | grep LOG
# IN_FILE="$(pwd)/input/softmax_2.in" taskset -c 1 make softmax_asimd | grep LOG
IN_FILE="$(pwd)/input/softmax_2.in" taskset -c 1 make softmax_sve | grep LOG
diff ./softmax.out.nosimd ./softmax.out.sve
# Softmax, no dump result
IN_FILE="$(pwd)/input/softmax.in" taskset -c 1 make softmax_nodump_nosve | grep LOG
# IN_FILE="$(pwd)/input/softmax.in" taskset -c 1 make softmax_nodump_asimd | grep LOG
IN_FILE="$(pwd)/input/softmax.in" taskset -c 1 make softmax_nodump_sve | grep LOG

# ReLU
IN_FILE="$(pwd)/input/relu.in" taskset -c 1 make relu_nosve | grep Calculation
IN_FILE="$(pwd)/input/relu.in" taskset -c 1 make relu_sve | grep Calculation
```
