#include <iostream>
#include <array>
#include <vector>

template <typename T>
const T& max(const T& lhs, const T& rhs)
{
    return (lhs >= rhs ? lhs : rhs);
}

template <>
const std::string& max<std::string>(const std::string& lhs, const std::string& rhs)
{
    return (lhs.size() >= rhs.size() ? lhs : rhs); 
}

template <typename T, typename Comp>
const T& max(const T& lhs, const T& rhs, Comp comp)
{
    return (comp(lhs, rhs) ? lhs : rhs);
}

template <typename T>
bool compare(const T& lhs, const T& rhs)
{
    std::cout << "compare func lhs = " << lhs << " rhs = " << rhs << std::endl;
    return lhs > rhs;
}

template <typename T>
struct Comparator
{
};

template <>
struct Comparator<float>
{
    bool operator()(const float& lhs, const float& rhs) const
    {
        std::cout << "Comparator float lhs = " << lhs << " rhs = " << rhs << std::endl;
        return lhs > rhs;
    }
};

int main()
{
    std::cout << max<int>(1, 2) << std::endl;
    std::cout << max(1.0, 2.2) << std::endl;
    std::cout << max<std::string>("a", "abc") << std::endl;
    std::cout << max<int>(10, 50, compare<int>) << std::endl;
    std::cout << max<float>(10.2, 10.3, Comparator<double>()) << std::endl;
}
