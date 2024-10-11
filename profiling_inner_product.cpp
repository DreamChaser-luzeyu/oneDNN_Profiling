#include <chrono>
#include <cstdio>
#include <dnnl.hpp>
#include <iostream>
#include <random>

#include "example_utils.hpp"

using namespace dnnl;
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::chrono::microseconds;
using std::chrono::duration_cast;

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
    int n, ic, oc;
    cout << "Please enter param n, ic, oc:";
    cin >> n >> ic >> oc;

    // 3---- Initialize matrix structs
    // 3.1-- Allocate matrix arr
    vector<float> src(n * ic);
    vector<float> weight(oc * ic);
    vector<float> bias(oc);
    vector<float> dst(n * oc);
    // 3.2-- Create oneDNN memory desc & mem
    auto src_md =
        memory::desc({ic, n}, memory::data_type::f32, memory::format_tag::ab);
    auto src_m = memory(src_md, eng);
    auto weight_md =
        memory::desc({ic, oc}, memory::data_type::f32, memory::format_tag::ab);
    auto weight_m = memory(weight_md, eng);
    auto bias_md =
        memory::desc({oc}, memory::data_type::f32, memory::format_tag::a);
    auto bias_m = memory(bias_md, eng);
    auto dst_md =
        memory::desc({n, oc}, memory::data_type::f32, memory::format_tag::ab);
    auto dst_m = memory(dst_md, eng);

    // 4---- Initialize matrix data
    // 4.1-- Get data
    cout << "Would you like to specify data(Y/n):";
    char choice;
    cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        // Get data from user input
        printf("src matrix (expected %d): ", n * ic);
        for(int i = 0; i < (n * ic); i++) cin >> src[i];
        printf("weight matrix (expected %d): ", oc * ic);
        for(int i = 0; i < (oc * ic); i++) cin >> weight[i];
        printf("bias matrix (expected %d): ", oc);
        for(int i = 0; i < oc; i++) cin >> bias[i];
    } else {
        // Generate random float values
        for(int i = 0; i < (n * ic); i++) src[i] = gen_rand();
        for(int i = 0; i < (oc * ic); i++) weight[i] = gen_rand();
        for(int i = 0; i < oc; i++) bias[i] = gen_rand();
    }
    // 4.2-- Write data to memory object
    write_to_dnnl_memory(src.data(), src_m);
    write_to_dnnl_memory(weight.data(), weight_m);
    write_to_dnnl_memory(bias.data(), bias_m);

    // 5---- Create inner product primitive & run
    // 5.1-- Create inner product primitive
    auto inner_product_pd = inner_product_forward::primitive_desc(
        eng, prop_kind::forward_training, src_md, weight_md, dst_md);
    auto inner_product_p = inner_product_forward(inner_product_pd);
    // 5.2-- Run primitive & timing
    auto start = std::chrono::system_clock::now();
    cout << "Start calculation" << endl;
    inner_product_p.execute(s, {{DNNL_ARG_SRC, src_m},
                                {DNNL_ARG_WEIGHTS, weight_m},
                                {DNNL_ARG_BIAS, bias_m},
                                {DNNL_ARG_DST, dst_m}});
    s.wait();
    auto end = std::chrono::system_clock::now();
    auto duration = end - start;
    cout << "Calculation costs "
         << duration_cast<microseconds>(duration).count()
         << " microseconds" << endl;


    return 0;
}