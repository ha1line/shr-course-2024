### Полиморфизм в функциях
Мы познакомились с концепцией полиморфизма, когда через единый интерфейс объекта мы можем получать уникальное поведение в рантайме для разных типов. Давайте вернёмся немного назад и рассмотрим примеры полиморфизма в обычных функциях.
Давайте напишем прототип функции `max()`, для `int`.

```cpp
int max(const int lhs, const int rhs)
{
    return (lhs >= rhs ? lhs : rhs);
}
```

Теперь если нам нужно реализовать тоже самое для типа `double` мы можем перегрузить ещё одну функцию, с именем `max`, но с другими типами:

```cpp
double max(const double lhs, const double rhs)
{
    return (lhs >= rhs ? lhs : rhs);
}
```

Теперь мы можем воспользоваться этими функциями, для поиска максимума для целочисленных типов и типов с плавающей точкой. Фактически, мы получим полиморфизм на уровне функций.

```cpp
#include <iostream>

int max(const int lhs, const int rhs)
{
    return (lhs >= rhs ? lhs : rhs);
}

double max(const double lhs, const double rhs)
{
    return (lhs >= rhs ? lhs : rhs);
}

int main()
{
	std::cout << max(5, 10) << std::endl;
	std::cout << max(2.0, 0) << std::endl;
}
```

Код будет работать корректно, а что если добавить свой пользовательский тип, который тоже должен попасть в функцию `max()`? Нам придётся добавлять ещё одну перегрузку и так для всех новых типов в нашей программе. Это неудобно и сильно перегружает код.
Чтобы решить эту проблему в С++ появился механизм шаблонов.

>[!Note]
>Шаблон — это рецепт, по которому на этапе компиляции программы будет сгенерирован код.

### Шаблоны функций
Так код нашей функции на шаблонах будет выглядеть вот так:

```cpp
#include <iostream>

template <typename T>
T max(const T& lhs, const T& rhs)
{
    return (lhs >= rhs ? lhs : rhs);
}
```

Здесь `template` — это ключевое слово для объявления шаблона, в `<>` указываются типы, в нашем случае мы объявили, что у нас есть один шаблонный параметр типа T (имя может быть разным) `typename T`.
Прочитав эту языковую конструкцию, компилятор сгенерирует нужный код, здесь мы приходим к первой особенности шаблонов — это **ленивый** механизм. Компилятор не будет генерировать шаблонный код, которого не требуется в программе.
Так код вида:

```cpp
#include <iostream>

template <typename T>
T max(const T& lhs, const T& rhs)
{
    return (lhs >= rhs ? lhs : rhs);
}

int main()
{
	std::cout << max(10, 50) << std::endl;
}
```

Преобразуется в такой код:

```cpp
#include <iostream>

template <typename T>
T max(const T& lhs, const T& rhs)
{
    return (lhs >= rhs ? lhs : rhs);
}

int max(const int& lhs, const int& rhs)
{
    return (lhs >= rhs ? lhs : rhs);
}

int main()
{
	std::cout << max(10, 50) << std::endl;
}
```

Компилятор сгенерировал код только для тех типов, которые мы вызвали, а не для всех существующих типов в С++, так добавив вызов функции `max` от `double`, у нас добавилась бы ещё одна перегрузка функции. Эта одна из иллюстраций «ленивости» шаблонов.
Этот механизм активно используется в С++, любой контейнер стандартной библиотеки — это шаблонный класс, который генерирует код под тот тип, который вы указали в программе, тоже самое и с функциями стандартной библиотеки.
Шаблоны — это одна из основных фичей языка С++ наравне с ручным выделением памяти и RAII.
Мы также можем явно указывать шаблонные параметры при вызове функции. Начиная с С++ 17 они умеют подставляться автоматически, за исключением некоторых случаев.

```cpp
#include <iostream>

template <typename T>
T max(const T& lhs, const T& rhs)
{
    return (lhs >= rhs ? lhs : rhs);
}

int main()
{
	std::cout << max<int>(10, 50) << std::endl;
}
```

### Специализации шаблонов
Итак у нас есть общий рецепт, по которому компилятор будет генерировать код, однако нам может понадобиться специфическое поведение для частных случаев. Например, условимся, что если в нашу функцию `max()` попадёт `std::string`, то сравнивать его нужно будет по параметру `size()`.
Для это существует механизм частичной специализации.

```cpp
#include <iostream>

template <typename T>
T max(const T& lhs, const T& rhs)
{
    return (lhs >= rhs ? lhs : rhs);
}

template <>
std::string max<std::string>(const std::string& lhs, const std::string& rhs)
{
    return (lhs.size() >= rhs.size() ? lhs : rhs);
}

int main()
{
	std::cout << max<int>(10, 50) << std::endl;
	std::cout << max("a", "abc") << std::endl;
}
```

Однако, такой вариант кода не соберется с ошибкой:
```
<source>: In function 'int main()':
<source>:18:25: error: no matching function for call to 'max(const char [2], const char [4])'
   18 |         std::cout << max("a", "abc") << std::endl;
      |                      ~~~^~~~~~~~~~~~
<source>:4:3: note: candidate: 'template<class T> T max(const T&, const T&)'
    4 | T max(const T& lhs, const T& rhs)
      |   ^~~
<source>:4:3: note:   template argument deduction/substitution failed:
<source>:18:25: note:   deduced conflicting types for parameter 'const T' ('char [2]' and 'char [4]')
   18 |         std::cout << max("a", "abc") << std::endl;
      |                      ~~~^~~~~~~~~~~~
Compiler returned: 1
```

Данная ошибка показывает нам ещё одну важную особенность шаблонов: компилятор не будет выполнять неявных конверсий типов без явного указания. В данном случае, не будет произведено неявного каста из `const char*` в `std::string`, если бы функция была бы не шаблонной, это **без проблем бы произошло**.
Чтобы код заработал, явно укажем, от какого типа мы вызываем функцию:

```cpp
#include <iostream>

template <typename T>
T max(const T& lhs, const T& rhs)
{
    return (lhs >= rhs ? lhs : rhs);
}

template <>
std::string max<std::string>(const std::string& lhs, const std::string& rhs)
{
    return (lhs.size() >= rhs.size() ? lhs : rhs);
}

int main()
{
	std::cout << max<int>(10, 50) << std::endl;
	std::cout << max<std::string>("a", "abc") << std::endl;
}
```

Давайте расширим наш код, добавив ещё один шаблонный параметр в нашу функцию — компаратор. Это будет функция или функциональный объект, который позволит получать максимум среди двух объектов с помощью специальной логики.
Пусть наш компаратор будет представлен шаблонной функцией `compare`. Возвращать она должна значение типа `bool`.

```cpp
#include <iostream>

template <typename T>
T max(const T& lhs, const T& rhs)
{
    return (lhs >= rhs ? lhs : rhs);
}

template <typename T, typename Comp>
T max(const T& lhs, const T& rhs, Comp comp)
{
    return (comp(lhs, rhs) ? lhs : rhs);
}

template <>
std::string max<std::string>(const std::string& lhs, const std::string& rhs)
{
    return (lhs.size() >= rhs.size() ? lhs : rhs);
}

template <typename T>
bool compare(const T& lhs, const T& rhs)
{
    std::cout << "Compate lhs = " << lhs << " rhs = " << rhs << std::endl;
    return lhs > rhs;
}

int main()
{
    std::cout << max<int>(10, 50) << std::endl;
    std::cout << max<std::string>("a", "abc") << std::endl;
    std::cout << max<int>(10, 50, compare<int>) << std::endl;
}
```

Код выведет:
```
50
abc
Compare lhs = 10 rhs = 50
50
```

Часто в языке C++ для решения таких задач используются функциональные объекты (функторы) — это более стандартное решение для такой задачи. Функциональный объект — это класс или структура с перегруженным `bool operator()()` с нужным набором параметров. Так для `max()` или `min()` там будет 2 параметра, а в случае `logical_not()` будет 1 параметр — всё зависит от алгоритма, в котором используется функтор.
Функторы для сравнения двух объектов еще называют компараторами.
Так мы плавно подошли к тому, что шаблонными могут быть не только функции, но и классы.
>[!Note]
>В параметрах шаблона могут быть явно указаны типы, следовательно и в случае специализаций шаблонными параметрами могут стать, например, числа.
>Так для класса `std::array<T, size>` вторым параметром передаётся размер контейнера — число типа `size_t`, а не тип выражения.
### Шаблонные классы
Давайте объявим шаблон класса `comparator`, который будет представлять из себя функциональный объект, выполняющий специфическую логику сравнения для всех типов.

```cpp
#include <iostream>

template <typename T>
T max(const T& lhs, const T& rhs)
{
    return (lhs >= rhs ? lhs : rhs);
}

template <typename T, typename Comp>
T max(const T& lhs, const T& rhs, Comp comp)
{
    return (comp(lhs, rhs) ? lhs : rhs);
}

template <>
std::string max<std::string>(const std::string& lhs, const std::string& rhs)
{
    return (lhs.size() >= rhs.size() ? lhs : rhs);
}

template <typename T>
bool compare(const T& lhs, const T& rhs)
{
    std::cout << "Compare lhs = " << lhs << " rhs = " << rhs << std::endl;
    return lhs > rhs;
}

template <typename T>
struct comparator
{
    bool operator()(const T& lhs, const T& rhs) const
    {
        std::cout << "Comparator lhs = " << lhs << " rhs = " << rhs << std::endl;
        return lhs > rhs;
    }
};

int main()
{
    std::cout << max<int>(10, 50) << std::endl;
    std::cout << max<std::string>("a", "abc") << std::endl;
    std::cout << max<int>(10, 50, compare<int>) << std::endl;
    std::cout << max<int>(10, 50, comparator<int>()) << std::endl;
}

```

Код выведет:
```
50
abc
Compare lhs = 10 rhs = 50
50
Comparator lhs = 10 rhs = 50
50
```

Шаблонные классы также могут иметь специализации, как и функции. Один из самых ярких примеров таких специализаций — это `std::vector<bool>` и `std::array<bool, size>`.

### Вычисления с помощью шаблонов
Теперь мы знаем о механизме шаблонов в С++, это фича, позволяющая генерировать код на этапе компиляции. Но раз мы можем генерировать код, то почему бы нам не проводить вычисления для некоторых параметров и констант также в компайл тайме, не тратя реальное время работы программы?
Давайте напишем шаблонный класс, который на этапе компиляции будет рассчитывать числа Фибоначчи.
Логика очень проста: в исходном классе мы должны делать шаблонную подстановку от значения, расчитанного в классе со специализацией (N-1) + в классе (N-2). Всё как в математической формуле чисел Фибоначчи. Вот так будет выглядеть код:

```cpp
#include <iostream>

template <int N>
struct fibonacci_calc
{
    static const unsigned long long value()
    {
        return fibonacci_calc<N - 1>::value() + fibonacci_calc<N - 2>::value();
    }
};

int main()
{
    std::cout << "Fibonacci(10) = " << fibonacci_calc<10>::value() << std::endl;
}

```

При попытке скомпилировать его мы получим следующую ошибку:
```
templates_and_comparators.cpp:8:16: fatal error: **recursive template instantiation exceeded maximum depth of 1024**
    8 |         return fibonacci_calc<N - 1>::value() + fibonacci_calc<N - 2>::value();
      |                ^
templates_and_comparators.cpp:8:39: note: in instantiation of member function 'fibonacci_calc<-1014>::value' requested here
    8 |         return fibonacci_calc<N - 1>::value() + fibonacci_calc<N - 2>::value();
      |                                       ^
templates_and_comparators.cpp:8:39: note: in instantiation of member function 'fibonacci_calc<-1013>::value' requested here
templates_and_comparators.cpp:8:39: note: in instantiation of member function 'fibonacci_calc<-1012>::value' requested here
templates_and_comparators.cpp:8:39: note: in instantiation of member function 'fibonacci_calc<-1011>::value' requested here
templates_and_comparators.cpp:8:39: note: in instantiation of member function 'fibonacci_calc<-1010>::value' requested here
templates_and_comparators.cpp:8:39: note: (skipping 1015 contexts in backtrace; use -ftemplate-backtrace-limit=0 to see all)
templates_and_comparators.cpp:8:39: note: in instantiation of member function 'fibonacci_calc<6>::value' requested here
templates_and_comparators.cpp:8:39: note: in instantiation of member function 'fibonacci_calc<7>::value' requested here
templates_and_comparators.cpp:8:39: note: in instantiation of member function 'fibonacci_calc<8>::value' requested here
templates_and_comparators.cpp:8:39: note: in instantiation of member function 'fibonacci_calc<9>::value' requested here
templates_and_comparators.cpp:34:60: note: in instantiation of member function 'fibonacci_calc<10>::value' requested here
   34 |     std::cout << "Fibonacci(10) = " << fibonacci_calc<10>::value() << std::endl;
      |                                                            ^
templates_and_comparators.cpp:8:16: note: use -ftemplate-depth=N to increase recursive template instantiation depth
    8 |         return fibonacci_calc<N - 1>::value() + fibonacci_calc<N - 2>::value();
      |  
```

Компилятор начал выполнять подстановку, начинаю от вызванного нами класса с N = 10, затем начали генерировать классы от N = 9, N = 8 и т. д. Так происходило бесконечно, потому что не было указана точка остановки. Шаблонный стек начал бесконечно расширяться и мы постепенно уперлись в его ограничение — 1024 шаблонные подстановки.
Чтобы исправить эту проблему и сделать наш алгоритм корректным, нам нужно ввести специализации для шаблонов для N = 0, N = 1.
Тогда наш код будет выглядеть вот так:

```cpp
#include <iostream>

template <int N>
struct fibonacci_calc
{
    static const unsigned long long value()
    {
        return fibonacci_calc<N - 1>::value() + fibonacci_calc<N - 2>::value();
    }
};


template <>
struct fibonacci_calc<0>
{
    static const unsigned long long value()
    {
        return 0;
    }
};

template <>
struct fibonacci_calc<1>
{
    static const unsigned long long value()
    {
        return 1;
    }
};

int main()
{
    std::cout << "Fibonacci(10) = " << fibonacci_calc<10>::value() << std::endl;
}

```

Теперь всё пройдет корректно, мы рассчитаем 10 число Фибоначчи на этапе компиляции, получим 55.
Важно отметить, что в рантайме у нас не будет создано ни одного объекта `fibonacci_calc`, он нужен здесь как обертка для `static` метода `value()`. 

>[!Note]
> Статические методы в классах можно воспринимать как обычные функции, которые привязаны к объекту. Они не требуют создания экземпляра класса для их использования.
### Спецификатор `constexpr`
Начиная с С++ 11 можно попросить компилятор посчитать значение констант или других переменных на этапе компиляции. Для этого их нужно пометить ключевым словом `constexpr`, важно отметить, что это **не гарантирует**, что выражение будет посчитано на этапе компиляции. Иногда рассчитать значение, не запуская программу, невозможно.

```cpp
#include <iostream>

int read_int()
{
    int foo;
    std::cin >> foo;
    return foo;
}

int main()
{
    // значение посчитается на этапе компиляции
    constexpr double pi = 3.14;
    // значение нельзя посчитать на этапе компиляции, CE
    constexpr int rInt = read_int();
}
```

Спецификатором `constexpr` могут быть помечены не только переменные, но и функции и методы. Так следующий код будет расcчитан на этапе компиляции:

```cpp
constexpr int sum (int a, int b)
{
	return a + b;
}

int main()
{
	constexpr int c = sum (5, 12);
}
```

С пониманием работы спецификатора `constexpr` мы можем дополнить код класса `fibonacci_calc`:

```cpp
#include <iostream>

template <int N>
struct fibonacci_calc
{
    static constexpr unsigned long long value()
    {
        return fibonacci_calc<N - 1>::value() + fibonacci_calc<N - 2>::value();
    }
};

template <>
struct fibonacci_calc<0>
{
    static constexpr unsigned long long value()
    {
        return 0;
    }
};

template <>
struct fibonacci_calc<1>
{
    static constexpr unsigned long long value()
    {
        return 1;
    }
};

int main()
{
    // значение расчитано на этапе компиляции
    constexpr unsigned long long f15 = fibonacci_calc<15>::value();
    std::cout << f15 << std::endl;
}
```
