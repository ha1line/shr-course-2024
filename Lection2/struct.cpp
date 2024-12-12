#include <iostream>
#include <string>

struct Dummy
{
    Dummy()
    {
        std::cout << "Dummy()" << std::endl;
    }
    Dummy(int f)
    {
        std::cout << "Dummy(int)" << std::endl;
    }
    ~Dummy()
    {
        std::cout << "~Dummy()" << std::endl;
    }
};

struct StringHolder
{
    StringHolder() :
        m_dummy(Dummy(5))
    {
        std::cout << "StringHolder()" << std::endl;
    }

    explicit StringHolder(const std::string& str) :
        m_str(str)
    {
    }

    explicit StringHolder(int d)
    {
        std::cout << "StringHolder(int)" << std::endl;
    }

    explicit StringHolder(size_t size, char c) :
        m_str(std::string(size, c))
    {
    }

    ~StringHolder()
    {
        std::cout << "~StringHolder()" << std::endl;
    }
    std::string m_str;
    // Since C++11
    const Dummy m_dummy = Dummy();
};

int main()
{
    // "Hello" -> can't find StringHolder(const char*)
    // StringHolder(const std::string&) не подходит, т.к. нет неявного преобразования из const char* в std::string
    // Это требует создания нового объекта
    // CE
    //StringHolder holder = "Hello";
    // CE, because of explicit
    // StringHolder holder = std::string("Hello");
    StringHolder holderStr = StringHolder(5, 'c');
    std::string str(std::string("hello"));
    std::string str1("Specific String");
    StringHolder holderInt(2.0);
}
