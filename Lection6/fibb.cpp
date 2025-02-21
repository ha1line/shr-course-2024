#include <iostream>

// 0,  1, (N - 2) + (N - 1)

template <size_t N>
struct fibonacci_calc
{
    static constexpr unsigned long long value()
    {
        return fibonacci_calc<N - 2>::value() + fibonacci_calc<N - 1>::value();
    }
};

template<>
struct fibonacci_calc<0>
{
    static constexpr unsigned long long value()
    {
        return 0;
    }
};

template<>
struct fibonacci_calc<1>
{
    static constexpr unsigned long long value()
    {
        return 1;
    }
};

constexpr int sum(int a, int b)
{
    if constexpr (true)
    {
        return 0;
    }
    return a + b;
}

int main()
{
    // N-ное число Фибоначчи считалось на этапе компиляции
    std::cout << fibonacci_calc<100>::value() << std::endl;// -> 10 число Фибоначчи
    constexpr unsigned long long f30 = fibonacci_calc<1>::value();
    std::cout << f30 << std::endl;
    constexpr int bar = 50;
    constexpr int foo = sum(bar, 50);
}
