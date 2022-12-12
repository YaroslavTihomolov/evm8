#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>

const int min_size = 256;
const int max_size = 8388608;
const int K = 10;



int* FillForward(int size) {
    int* array;
    array = new int[size];
    for (int j = 0; j < size; j++)
        array[j] = (j + 1);
    array[size - 1] = 0;
    return array;
}

int* FillReverse(int size) {
    int* array;
    array = new int[size];
    for (int j = size - 1; j >= 0; j--)
        array[j] = j - 1;
    array[0] = size - 1;
    return array;
}

int* FillRandom(int size) {
    int* array;
    array = new int[size];
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

void ProcWarmingUp(const int* &array, int size) {
    for (int k = 0, i = 0; i < size; i++) k = array[k];
}

double Detour(const int* array, int size) {
    union ticks{
        unsigned long long t64;
        struct s32 { long th, tl; } t32;
    } start{}, end{};
    ProcWarmingUp(array, size);
    double sum = 0;
    asm volatile ("rdtsc\n":"=a"(start.t32.th), "=d"(start.t32.tl) :: "memory");
    for (int k = 0, h = 0; h < size * K; h++) {
        k = array[k];// std::cout << k << ' ';
        sum += k;
    }
    asm volatile ("rdtsc\n":"=a"(end.t32.th), "=d"(end.t32.tl) :: "memory");
    std::cout << "Count of elements:" << size;
    std::cout << "       Time taken:" <<(end.t64-start.t64) / (size * K) << "          ";
    delete [] array;
    return sum;
}

int main() {
    int* array;
    double sum;
    for (int i = min_size; i <= max_size; i *= 2) {
        array = FillForward(i);
        sum = Detour(array, i);
        std::cout << sum << '\n';

        array = FillReverse(i);
        sum = Detour(array, i);
        std::cout << sum << '\n';


        array = FillRandom(i);
        sum = Detour(array, i);
        std::cout << sum << '\n';
    }
    return 0;
}
