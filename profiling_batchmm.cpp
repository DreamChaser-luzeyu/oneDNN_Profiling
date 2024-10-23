/*******************************************************************************
* Copyright 2020-2022 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/


#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <fstream>

#include "example_utils.hpp"
#include "oneapi/dnnl/dnnl.hpp"

using namespace dnnl;

using tag = memory::format_tag;
using dt = memory::data_type;

using std::chrono::duration_cast;
using std::chrono::microseconds;

void matmul_example(dnnl::engine::kind engine_kind) {

    // Create execution dnnl::engine.
    dnnl::engine engine(engine_kind, 0);

    // Create dnnl::stream.
    dnnl::stream engine_stream(engine);

    // Tensor dimensions.
    memory::dim MB = 3, // batch size
            M = 128, K = 256, N = 512;

    std::cout << "[BATCHMM] Please enter param MB(Batch size), M, N: ";
    std::cin >> MB >> M >> N;

    // Source (src), weights, bias, and destination (dst) tensors dimensions.
    memory::dims src_dims = {MB, M, K};
    memory::dims weights_dims = {MB, K, N};
    // memory::dims bias_dims = {1, 1, N};
    memory::dims dst_dims = {MB, M, N};

    // Allocate buffers.
    std::vector<float> src_data(product(src_dims));
    std::vector<float> weights_data(product(weights_dims));
    // std::vector<float> bias_data(product(bias_dims));
    std::vector<float> dst_data(product(dst_dims));

    // Initialize src, weights, bias.
    std::generate(src_data.begin(), src_data.end(), []() {
        static int i = 0;
        return std::cos(i++ / 10.f);
    });
    std::generate(weights_data.begin(), weights_data.end(), []() {
        static int i = 0;
        return std::sin(i++ * 2.f);
    });
    // std::generate(bias_data.begin(), bias_data.end(), []() {
    //     static int i = 0;
    //     return std::tanh(float(i++));
    // });

    // Create memory descriptors and memory objects for src, weights, bias, and
    // dst.
    auto src_md = memory::desc(src_dims, dt::f32, tag::abc);
    auto weights_md = memory::desc(weights_dims, dt::f32, tag::abc);
    // auto bias_md = memory::desc(bias_dims, dt::f32, tag::abc);
    auto dst_md = memory::desc(dst_dims, dt::f32, tag::abc);

    auto src_mem = memory(src_md, engine);
    auto weights_mem = memory(weights_md, engine);
    // auto bias_mem = memory(bias_md, engine);
    auto dst_mem = memory(dst_md, engine);

    // Write data to memory object's handles.
    write_to_dnnl_memory(src_data.data(), src_mem);
    write_to_dnnl_memory(weights_data.data(), weights_mem);
    // write_to_dnnl_memory(bias_data.data(), bias_mem);

    // *** RELU is disabled here
    // Create primitive post-ops (ReLU).
    // const float alpha = 0.f;
    // const float beta = 0.f;
    post_ops matmul_ops;
    // matmul_ops.append_eltwise(algorithm::eltwise_relu, alpha, beta);
    primitive_attr matmul_attr;
    matmul_attr.set_post_ops(matmul_ops);
    // --- Create primitive descriptor.
    // *** Do not use bias here
    // auto matmul_pd = matmul::primitive_desc(
    //         engine, src_md, weights_md, bias_md, dst_md, matmul_attr);
    auto matmul_pd = matmul::primitive_desc(
            engine, src_md, weights_md, dst_md, matmul_attr);

    // --- Create the primitive.
    auto matmul_prim = matmul(matmul_pd);

    // --- Primitive arguments.
    std::unordered_map<int, memory> matmul_args;
    matmul_args.insert({DNNL_ARG_SRC, src_mem});
    matmul_args.insert({DNNL_ARG_WEIGHTS, weights_mem});
    // matmul_args.insert({DNNL_ARG_BIAS, bias_mem});
    matmul_args.insert({DNNL_ARG_DST, dst_mem});

    std::cout << "Start calculation" << std::endl;
    auto start = std::chrono::system_clock::now();
    // --- Primitive execution: matrix multiplication with ReLU.
    matmul_prim.execute(engine_stream, matmul_args);
    // Wait for the computation to finalize.
    engine_stream.wait();
    auto end = std::chrono::system_clock::now();
    auto duration = end - start;
    std::cout << "[LOG] Calculation costs "
         << duration_cast<microseconds>(duration).count() << " microseconds"
         << std::endl;
    // Read data from memory object's handle.
    read_from_dnnl_memory(dst_data.data(), dst_mem);

    if (getenv("BUILD_ARCH")) {
        std::cout << "[LOG] Dumping result..." << std::endl;
        char str_buf[128];
        char* build_arch_str = getenv("BUILD_ARCH");
        strcpy(str_buf, "batchmm.out.");
        const char* file_name_str = strcat(str_buf, build_arch_str);
        std::fstream f;
        f.open(file_name_str, std::ios::out);
        for (int i = 0; i < dst_data.size(); i++) {
            f << dst_data[i] << " ";
        }
        f << std::endl;
        f.flush();
        f.close();
    }
}

int main(int argc, char **argv) {
    return handle_example_errors(matmul_example, parse_engine_kind(argc, argv));
}