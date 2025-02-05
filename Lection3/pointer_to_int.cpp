#include <iostream>
#include <algorithm>
#include <string>

class PointerToInt
{
public:
    PointerToInt() = default;
    PointerToInt(double) = delete;
    PointerToInt(const int num) :
        m_ptr(new int(num))
    {
    }

    // Copy constructor
    PointerToInt(const PointerToInt& o) :
        m_ptr((o.m_ptr != nullptr) ? (new int(*o.m_ptr)) : (nullptr))
    {
        // lhs = (условие) ? (true) : (false)
    }

    // Copy and Swap
    // this
    // 1. Создать копию
    // 2. Поменять значения this и копии
    // Copy assignment operator
    PointerToInt& operator=(const PointerToInt& o)
    {
        if (this != &o)
        {
            swap(o);
        }
        return *this;
    }

    int& operator*()
    {
        return *m_ptr;
    }

    const int& operator*() const
    {
        return *m_ptr;
    }

    int* Get()
    {
        return m_ptr;
    }

    const int* Get() const
    {
        return m_ptr;
    }

    ~PointerToInt()
    {
        delete m_ptr;
    }
private:
    // PointetToInt -> copy
    void swap(PointerToInt copy)
    {
        std::swap(m_ptr, copy.m_ptr);
    }
private:
    int* m_ptr = nullptr;
};

// Copy PointerToInt -> new()
// Copy Assignment = -> new()

class PointerHolder
{
    PointerToInt m_ptr = PointerToInt(5);
};

int main()
{
    // Copy Constructor
    {
        PointerToInt p(5);
        PointerToInt p1(p);
        std::cout << "first pointer=" << *p.Get() << " second pointer=" << *p1.Get() << std::endl;
        std::cout << "address of fp=" << p.Get() << " address sp=" << p1.Get() << std::endl;
    }
    {
        PointerToInt p(5);
        PointerToInt p1(6);
        //
        p = p1;
        std::cout << "first pointer=" << *p.Get() << " second pointer=" << *p1.Get() << std::endl;
        std::cout << "address of fp=" << p.Get() << " address sp=" << p1.Get() << std::endl;
        p = p;
    }
    // Default 
    {
        PointerToInt p;
        PointerToInt p1(p);
        std::cout << (nullptr == p1.Get()) << std::endl;
    }
    {
        PointerToInt p;
        PointerToInt p1(5);
        p1 = p;
        std::cout << (nullptr == p1.Get()) << std::endl;
    }

    // Operator*
    {
        PointerToInt p;
        if (p.Get())
        {
            std::cout << *p << std::endl;
        }
    }
    // const PointerToInt
    {
        const PointerToInt p(5);
        std::cout << *p << std::endl;
    }
}
