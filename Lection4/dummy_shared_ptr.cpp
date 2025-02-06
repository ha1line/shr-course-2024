#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

// 1. Global counter/ Global map of counters
// 2. Хранить счетчик рядом с указателем

/**
 * @brief Наивная реализация класса shared_ptr
 * 
 */
class PointerToInt
{
public:
    PointerToInt() = default;
    PointerToInt(const int num) :
        m_ptr(new int(num)),
        m_counter(new size_t(1))
    {
    }

    // Copy constructor
    PointerToInt(const PointerToInt& o)
    {
        m_ptr = o.m_ptr;
        m_counter = o.m_counter;
        if (m_counter)
        {
            ++(*m_counter);
        }
    }

    // Copy assignment operator
    PointerToInt& operator=(const PointerToInt& o)
    {
        if (m_ptr != o.m_ptr)
        {
            if (m_counter && --(*m_counter) == 0)
            {
                delete m_ptr;
                delete m_counter;
            }
            swap(o);

            return *this;
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
        if (m_counter && --(*m_counter) == 0)
        {
            delete m_ptr;
            delete m_counter;
        }
    }
private:
    void swap(PointerToInt copy)
    {
        std::swap(m_ptr, copy.m_ptr);
        std::swap(m_counter, copy.m_counter);
    }
private:
    int* m_ptr = nullptr;
    size_t* m_counter = nullptr;
};

// Если counter размещен на стеке, то при обращении и работе с PointerToInt мы получим UB
// т. к. память будет сразу же очищена при выходе из функции
PointerToInt get_pointer()
{
    PointerToInt p(5);
    std::cout << p.Get() << std::endl;
    return p;
}

int main()
{
    {
        boost::shared_ptr<int> ptr = boost::make_shared<int>(5); // use_count 1
        boost::shared_ptr<int> ptr4(ptr); // use_count 2

        boost::shared_ptr<int> ptr2 = boost::make_shared<int>(4); // use_count 1
        boost::shared_ptr<int> ptr3(ptr2); // use_count 2
        std::cout << ptr.use_count() << std::endl;
        std::cout << ptr3.use_count() << std::endl;

        ptr = ptr3;
        std::cout << ptr4.use_count() << std::endl;
        std::cout << ptr3.use_count() << std::endl;
    }

    {
        PointerToInt p(6);
        PointerToInt p1(5);
        p = p1;
    }
}
