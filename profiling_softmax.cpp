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
#include <cstdlib>
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

void softmax_example(dnnl::engine::kind engine_kind) {
    // 1---- Initialize oneDNN engine & stream
    // Create execution dnnl::engine.
    dnnl::engine engine(engine_kind, 0);
    // Create dnnl::stream.
    dnnl::stream engine_stream(engine);

    // 2---- Get data
    // Tensor dimensions.
    memory::dim N = 3, // batch size
            IC = 1000; // channels
    std::cout << "[SOFTMAX] Please enter param N(Batch size), IC(Channel size): ";
    std::cin >> N >> IC;
    // Source (src) and destination (dst) tensors dimensions.
    memory::dims src_dims = {N, IC};

    // Allocate buffer.
    std::vector<float> src_data(product(src_dims));
    std::generate(src_data.begin(), src_data.end(), []() {
        static int i = 0;
        return std::cos(i++ / 10.f);
    });


    // 3---- Initialize descriptors & tensor data
    // Create src memory descriptor and memory object.
    auto src_md = memory::desc(src_dims, dt::f32, tag::nc);
    auto dst_md = memory::desc(src_dims, dt::f32, tag::nc);
    auto src_mem = memory(src_md, engine);
    // Write data to memory object's handle.
    write_to_dnnl_memory(src_data.data(), src_mem);
    // Softmax axis.
    const int axis = 1;

    // 4---- Create primitive & run
    // Create primitive descriptor.
    auto softmax_pd = softmax_forward::primitive_desc(engine,
            prop_kind::forward_training, algorithm::softmax_accurate, src_md,
            dst_md, axis);

    // Create the primitive.
    auto softmax_prim = softmax_forward(softmax_pd);

    // Primitive arguments. Set up in-place execution by assigning src as DST.
    std::unordered_map<int, memory> softmax_args;
    softmax_args.insert({DNNL_ARG_SRC, src_mem});
    softmax_args.insert({DNNL_ARG_DST, src_mem});

    std::cout << "Start calculation" << std::endl;
    auto start = std::chrono::system_clock::now();
    // Primitive execution.
    softmax_prim.execute(engine_stream, softmax_args);
    // Wait for the computation to finalize.
    engine_stream.wait();

    auto end = std::chrono::system_clock::now();
    auto duration = end - start;
    std::cout << "[LOG] Calculation costs "
         << duration_cast<microseconds>(duration).count() << " microseconds"
         << std::endl;

    // Read data from memory object's handle.
    read_from_dnnl_memory(src_data.data(), src_mem);

    if (getenv("BUILD_ARCH")) {
        std::cout << "[LOG] Dumping result..." << std::endl;
        char str_buf[128];
        char* build_arch_str = getenv("BUILD_ARCH");
        strcpy(str_buf, "softmax.out.");
        const char* file_name_str = strcat(str_buf, build_arch_str);
        std::fstream f;
        f.open(file_name_str, std::ios::out);
        for (int i = 0; i < src_data.size(); i++) {
            f << src_data[i] << " ";
        }
        f << std::endl;
        f.flush();
        f.close();
    }
}

int main(int argc, char **argv) {
    return handle_example_errors(
            softmax_example, validate_engine_kind(dnnl::engine::kind::cpu));
}