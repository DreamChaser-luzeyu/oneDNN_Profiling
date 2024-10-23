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

void sum_example(dnnl::engine::kind engine_kind) {

    // Create execution dnnl::engine.
    dnnl::engine engine(engine_kind, 0);

    // Create dnnl::stream.
    dnnl::stream engine_stream(engine);

    // Tensor dimensions.
    memory::dim N = 3, // batch size
            IC = 3, // channels
            IH = 227, // tensor height
            IW = 227; // tensor width

    std::cout << "[MATSUM] Please enter param N(Batch size), IC(Channel size), IH, IW: ";
    std::cin >> N >> IC >> IH >> IW;

    // Source (src) and destination (dst) tensors dimensions.
    memory::dims src_dims = {N, IC, IH, IW};

    // Allocate buffers.
    std::vector<float> src_data(product(src_dims));
    std::vector<float> dst_data(product(src_dims));

    // Initialize src.
    std::generate(src_data.begin(), src_data.end(), []() {
        static int i = 0;
        return std::cos(i++ / 10.f);
    });

    // Number of src tensors.
    const int num_src = 10;

    // Scaling factors.
    std::vector<float> scales(num_src);
    std::generate(scales.begin(), scales.end(),
            [](int n = 0) { return sin(float(n)); });

    // Create an array of memory descriptors and memory objects for src tensors.
    std::vector<memory::desc> src_md;
    std::vector<memory> src_mem;

    for (int n = 0; n < num_src; ++n) {
        auto md = memory::desc(src_dims, dt::f32, tag::nchw);
        auto mem = memory(md, engine);

        // Write data to memory object's handle.
        write_to_dnnl_memory(src_data.data(), mem);

        src_md.push_back(md);
        src_mem.push_back(mem);
    }

    // Create primitive descriptor.
    auto sum_pd = sum::primitive_desc(engine, scales, src_md);

    // Create the primitive.
    auto sum_prim = sum(sum_pd);

    // Create memory object for dst.
    auto dst_mem = memory(sum_pd.dst_desc(), engine);

    // Primitive arguments.
    std::unordered_map<int, memory> sum_args;
    sum_args.insert({DNNL_ARG_DST, dst_mem});
    for (int n = 0; n < num_src; ++n) {
        sum_args.insert({DNNL_ARG_MULTIPLE_SRC + n, src_mem[n]});
    }

    std::cout << "Start calculation" << std::endl;
    auto start = std::chrono::system_clock::now();
    // Primitive execution: sum.
    sum_prim.execute(engine_stream, sum_args);
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
    return handle_example_errors(sum_example, parse_engine_kind(argc, argv));
}