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

#include "example_utils.hpp"
#include "oneapi/dnnl/dnnl.hpp"

using namespace dnnl;

using tag = memory::format_tag;
using dt = memory::data_type;

using std::chrono::duration_cast;
using std::chrono::microseconds;

void eltwise_example(dnnl::engine::kind engine_kind) {
    // 1---- Create engine & stream
    // Create execution dnnl::engine.
    dnnl::engine engine(engine_kind, 0);
    // Create dnnl::stream.
    dnnl::stream engine_stream(engine);

    // 2---- Get data
    // Tensor dimensions.
    memory::dim N = 3, // batch size
            IC = 3, // channels
            IH = 227, // tensor height
            IW = 227; // tensor width
    std::cout << "[RELU] Please enter param N(Batch size), IC, IH, IW:";
    std::cin >> N >> IC >> IH >> IW;
    // Source (src) and destination (dst) tensors dimensions.
    memory::dims src_dims = {N, IC, IH, IW};
    memory::dims dst_dims = {N, IC, IH, IW};
    // Allocate buffers. In this example, out-of-place primitive execution is
    // demonstrated since both src and dst are required for later backward
    // propagation.
    std::vector<float> src_data(product(src_dims));
    std::vector<float> dst_data(product(dst_dims));
    // Initialize src tensor.
    std::generate(src_data.begin(), src_data.end(), []() {
        static int i = 0;
        return std::cos(i++ / 10.f);
    });
    // Create src and dst memory descriptors and memory objects.
    auto src_md = memory::desc(src_dims, dt::f32, tag::nchw);
    auto dst_md = memory::desc(dst_dims, dt::f32, tag::nchw);
    auto src_mem = memory(src_md, engine);
    auto dst_mem = memory(dst_md, engine);
    // Write data to memory object's handle.
    write_to_dnnl_memory(src_data.data(), src_mem);

    // 3---- Create primitive
    // Create primitive descriptor.
    auto eltwise_pd = eltwise_forward::primitive_desc(engine,
            prop_kind::forward_training, algorithm::eltwise_relu, src_md,
            dst_md, 0.f, 0.f);
    // Create the primitive.
    auto eltwise_prim = eltwise_forward(eltwise_pd);
    // Primitive arguments.
    std::unordered_map<int, memory> eltwise_args;
    eltwise_args.insert({DNNL_ARG_SRC, src_mem});
    eltwise_args.insert({DNNL_ARG_DST, dst_mem});

    // 4---- Do calc & profile time
    std::cout << "Start calculation" << std::endl;
    auto start = std::chrono::system_clock::now();
    // Primitive execution: element-wise (ReLU).
    eltwise_prim.execute(engine_stream, eltwise_args);
    // Wait for the computation to finalize.
    engine_stream.wait();
    auto end = std::chrono::system_clock::now();
    auto duration = end - start;
    std::cout << "Calculation costs "
         << duration_cast<microseconds>(duration).count() << " microseconds"
         << std::endl;

    // Read data from memory object's handle.
    read_from_dnnl_memory(dst_data.data(), dst_mem);
}

int main(int argc, char **argv) {
    return handle_example_errors(
            eltwise_example, parse_engine_kind(argc, argv));
}