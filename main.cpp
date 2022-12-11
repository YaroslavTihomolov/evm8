#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>

const int min_size = 32;
const int max_size = 8388608;
const int K = 1;



std::vector<int> FillForward(int size) {
    std::vector<int> array;
    array.resize(size);
    for (int j = 0; j <= size; j++)
        array[j] = (j + 1);
    array[size - 1] = 0;
    return array;
}

std::vector<int> FillReverse(int size) {
    std::vector<int> array;
    array.resize(size);
    for (int j = size - 1; j >= 0; j--)
        array[j] = j - 1;
    array[0] = size - 1;
    return array;
}

void PrintArray(const std::vector<int> &array) {
    for (auto i: array)
        std::cout << i << " ";
    std::cout << "\n";
}

std::vector<int> FillRandom(int size) {
    std::vector<int> array;
    array.resize(size);
    for (int i = 0; i < size; i++)
        array[i] = -1;
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, size);
    int x, index = 0;
    for (int i = 0; i < size - 1; i++) {
        do {
            x = distribution(generator);
        } while (array[x] != -1 || x == index);
        array[index] = x;
        index = x;
    }
    array[index] = 0;
    return array;
}

void ProcWarmingUp(const std::vector<int> &array) {
    for (int k = 0, i = 0; i < array.size(); i++) k = array[k];
}

void Detour(const std::vector<int> &array) {
    union ticks{
        unsigned long long t64;
        struct s32 { long th, tl; } t32;
    } start{}, end{};
    ProcWarmingUp(array);
    asm("rdtsc\n":"=a"(start.t32.th),"=d"(start.t32.tl));
    for (int k = 0, h = 0; h < array.size() * K; h++) {k = array[k]; //std::cout << k << ' ';
    }
    asm("rdtsc\n":"=a"(end.t32.th),"=d"(end.t32.tl));
    std::cout << "Count of elements:" << array.size();
    std::cout << "       Time taken:" <<(end.t64-start.t64) / (array.size() * K) << '\n';
}

int main() {
    std::vector<int> array;
    for (int i = min_size; i <= max_size; i *= 2) {
        array = FillForward(i);
        Detour(array);
        //array.clear();

        array = FillReverse(i);
        Detour(array);
        //array.clear();

        array = FillRandom(i);
        Detour(array);
    }
    return 0;
}
