#include <iostream>
#include <string>

// Скопировать и поменять
// void foo(std::string)

// Скопировать и не поменять
// void foo(const std::string)

// Не скопировать и поменять
// void foo(std::string&)

// Не скопировать и не поменять
// void foo(const std::string&)

// rvlaue -> lvalue
void print_str(const std::string& str)
{
    std::cout << str << std::endl;
}

// UB
const std::string& get_large_string()
{
    const std::string str(100, 'b');
    return str;
}

struct StringHolder
{
    StringHolder()
    {
        std::cout << "String Holder" << std::endl;
    }

    ~StringHolder()
    {
        std::cout << "~StringHolder" << std::endl;
    }

    std::string m_str = "Hello World!";
};

// rvalue StringHolder() -> lvalue holder
// when return lvalue -> rvalue
const StringHolder& get_str(const StringHolder& holder)
{
    std::cout << "get_str" << std::endl;
    return holder;
    // holder -> lvalue
}

int main()
{
    // literals, lvalue, rvalue
    {
        // int - types, a - name, 5 - literal
        const int a = 5;
        // type — std::string, str — name, "Hello World!" -> const char* -> std::string
        // str — lvalue, "Hello World!" - rvalue, literal
        const std::string str = "Hello World!";
        // str1 — lvalue, std::string("Hello") - rvalue
        const std::string str1 = std::string("Hello");
        std::cout << &str << std::endl;
    }
    // refs
    {
        int num = 10;
        // refNum new name for num
        int& refNum = num;
        ++refNum;
        std::cout << refNum << std::endl;
    }
    // refs
    {
        int x = 30;
        int y = 50;
        // OK
        int& ref = x;
        std::cout << ref << std::endl;
        // x = y
        ref = y;
        std::cout << x << y << std::endl;
        // 50 50
        std::cout << &x << &ref << std::endl;
    }
    // crefs
    {
        int i = 5;
        const int& constRef = i;
    }
    // crefs, lifetime
    {
        // Large str
        const std::string str(100, 'a');
        print_str(str);
        // "Hello world!" - временный
        print_str("Hello world!");
        // std::string("Hello World!") - rvalue
        print_str(std::string("Hello World!"));
        // :63 std::string("Hello World!") -> print_str
        const std::string& ref = get_large_string();
    }
    // StringHolder
    {
        // rvalue -> StringHolder() -> lvalue in get_str(holder) -> return -> rvalue -> UB
        const StringHolder& holder = get_str(StringHolder());
        // UB -> holder.m
        std::cout << "Hello " << holder.m_str << std::endl;
    }
}

