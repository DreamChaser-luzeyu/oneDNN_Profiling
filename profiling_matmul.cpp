#include <chrono>
#include <cstdio>
#include <dnnl.hpp>
#include <iostream>
#include <random>

#include "example_utils.hpp"
#include "oneapi/dnnl/dnnl.hpp"

#define ENABLE_BIAS 1

using namespace dnnl;
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::chrono::duration_cast;
using std::chrono::microseconds;

float gen_rand() {
    std::normal_distribution<double> u(10, 2);
    std::default_random_engine e(time(NULL));
    return u(e);
}

int main() {
    // 1---- Initialize oneDNN engine & stream
    engine eng(engine::kind::cpu, 0);
    stream s(eng);

    // 2---- Get matrix size
    int m, k, n;
    cout << "Please enter param m, k, n:";
    cin >> m >> k >> n;

    // 3---- Initialize matrix structs
    // 3.1-- Allocate matrix arr
    vector<float> src(m * k);
    vector<float> weight(k * n);
#if ENABLE_BIAS
    vector<float> bias(m * n);
#endif
    vector<float> dst(m * n);
    // 3.2-- Create oneDNN memory desc & mem
    auto src_md =
        memory::desc({m, k}, memory::data_type::f32, memory::format_tag::ab);
    auto src_m = memory(src_md, eng);
    auto weight_md =
        memory::desc({k, n}, memory::data_type::f32, memory::format_tag::ab);
    auto weight_m = memory(weight_md, eng);
#if ENABLE_BIAS
    auto bias_md =
        memory::desc({m, n}, memory::data_type::f32, memory::format_tag::ab);

    auto bias_m = memory(bias_md, eng);
#endif
    auto dst_md =
        memory::desc({m, n}, memory::data_type::f32, memory::format_tag::ab);
    auto dst_m = memory(dst_md, eng);

    // 4---- Initialize matrix data
    // 4.1-- Get data
    cout << "Would you like to specify data(Y/n):";
    char choice;
    cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        // Get data from user input
        printf("src matrix (expected %d): ", m * k);
        for (int i = 0; i < (m * k); i++) cin >> src[i];
        printf("weight matrix (expected %d): ", k * n);
        for (int i = 0; i < (k * n); i++) cin >> weight[i];
#if ENABLE_BIAS
        printf("bias matrix (expected %d): ", m * n);
        for (int i = 0; i < (m * n); i++) cin >> bias[i];
#endif
    } else {
        // Generate random float values
        for (int i = 0; i < (m * k); i++) src[i] = gen_rand();
        for (int i = 0; i < (k * n); i++) weight[i] = gen_rand();
#if ENABLE_BIAS
        for (int i = 0; i < (m * n); i++) bias[i] = gen_rand();
#endif
    }
    // 4.2-- Write data to memory object
    write_to_dnnl_memory(src.data(), src_m);
    write_to_dnnl_memory(weight.data(), weight_m);
#if ENABLE_BIAS
    write_to_dnnl_memory(bias.data(), bias_m);
#endif

    // 5---- Create matrix multiplication primitive & run
    // 5.1-- Create matrix multiplication primitive
    auto matmul_pd = matmul::primitive_desc(eng, src_md, weight_md,
#if ENABLE_BIAS
                                            bias_md,
#endif
                                            dst_md);
    auto matmul_p = matmul(matmul_pd);
    // 5.2-- Run primitive & timing
    auto start = std::chrono::system_clock::now();
    cout << "Start calculation" << endl;
    matmul_p.execute(s, {
        {DNNL_ARG_SRC, src_m}, 
        {DNNL_ARG_WEIGHTS, weight_m},
#if ENABLE_BIAS
            {DNNL_ARG_BIAS, bias_m},
#endif
        {DNNL_ARG_DST, dst_m}
    });
    s.wait();
    auto end = std::chrono::system_clock::now();
    auto duration = end - start;
    cout << "Calculation costs "
         << duration_cast<microseconds>(duration).count() << " microseconds"
         << endl;

    // 6---- Show calculation result
    if (choice == 'y' || choice == 'Y') {
        read_from_dnnl_memory(dst.data(), dst_m);
        std::cout << "Dst matrix: " << std::endl;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                std::cout << dst[i * n + j] << " ";
            }
            std::cout << std::endl;
        }
    }

    return 0;
}