#include <cstring>
#include <vector>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <random>
#include "omp.h"

const int min = 0;
const int max = 1000000000;
std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(min, max);


void fillUpRandomly(int *arr, int size) {
    generator.seed(0);
    for (int i = 0; i < size; i++)
        arr[i] = distribution(generator);
}

int isSorted(const int *a, int size) {
    for (int i = 0; i < size - 1; i++)
        if (a[i] > a[i + 1])
            return 0;

    return 1;
}


void mergeSortAux(int *arr, const int n, int *tmp) {
    int i = 0;
    int j = n / 2;
    int ti = 0;

    while (i < n / 2 && j < n) {
        if (arr[i] < arr[j]) {
            tmp[ti] = arr[i];
            ti++;
            i++;
        } else {
            tmp[ti] = arr[j];
            ti++;
            j++;
        }
    }
    while (i < n / 2) {
        tmp[ti] = arr[i];
        ti++;
        i++;
    }

    while (j < n) {
        tmp[ti] = arr[j];
        ti++;
        j++;
    }

    std::memcpy(arr, tmp, n * sizeof(int));
}


void mergeSort(int *arr, const int n, int *tmp, const int task_size) {
    if (n < 2)
        return;

#pragma omp task shared(arr, task_size, n, tmp) if (n > task_size) default(none)
    mergeSort(arr, n / 2, tmp, task_size);

#pragma omp task shared(arr, task_size, n, tmp) if (n > task_size) default(none)
    mergeSort(arr + (n / 2), n - (n / 2), tmp + n / 2, task_size);

#pragma omp taskwait
    mergeSortAux(arr, n, tmp);
}

struct Rule {
    int array_size;
    int threads_num;
    int task_size;
};

void init_merge_sort_tests() {
    std::vector<Rule> rules = {
            {25000000, 4,  1 << 8,},
            {25000000, 8,  1 << 8,},
            {25000000, 16, 1 << 8,},
            {25000000, 32, 1 << 8,},
            {25000000, 64, 1 << 8,},
    };

    omp_set_dynamic(0);
    std::for_each(rules.begin(), rules.end(), [](Rule &rule) {
        srand(0);
        omp_set_num_threads(rule.threads_num);

        int *arr = new int[rule.array_size];
        int *tmp = new int[rule.array_size];
        fillUpRandomly(arr, rule.array_size);
        fillUpRandomly(tmp, rule.array_size);

        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

#pragma omp parallel shared(arr, rule, tmp) default(none)
        {
#pragma omp single
            mergeSort(arr, rule.array_size, tmp, rule.task_size);
        }

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();


        std::cout << "Array size: " << rule.array_size << ". Task size: " << rule.task_size << ". Threads: "
                  << rule.threads_num;;
        std::cout << " Time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " (mcs)"
                  << std::endl;

        if (!isSorted(arr, rule.array_size)) {
            std::cerr << "Error sorting array";
            delete[] arr;
            delete[] tmp;
            exit(-1);
        }

        delete[] arr;
        delete[] tmp;
    });
}

int main() {
    std::cout << "Starting merge sort tests..." << std::endl;
    init_merge_sort_tests();
    return 0;
}
