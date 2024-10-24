#include <stdlib.h>

#include <chrono>
#include <cstdio>
#include <iostream>

#ifndef VEC_SIZE
#define VEC_SIZE 128
#endif

#ifndef VEC_NUM
#define VEC_NUM 819200
#endif

using std::cin;
using std::cout;
using std::endl;
using std::chrono::duration_cast;
using std::chrono::microseconds;

void __attribute__ ((noinline)) fun(double *a, double *b, int size) {
    for (int i = 0; i < size; ++i) {
        b[i] += a[i];
    }
}

#pragma optimize("", off)
// #pragma GCC push_options
// #pragma GCC optimize ("O0")
int main() {
    int i;

    double *a = (double *)malloc(sizeof(double) * VEC_SIZE);
    double *b = (double *)malloc(sizeof(double) * VEC_SIZE);

    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < VEC_NUM; i++) fun(a, b, VEC_SIZE);
    auto end = std::chrono::system_clock::now();
    auto duration = end - start;
    cout << "Calculation costs "
         << duration_cast<microseconds>(duration).count() << " microseconds"
         << endl;

    printf("Done.\n");
}
// #pragma GCC pop_options
#pragma optimize("", on)