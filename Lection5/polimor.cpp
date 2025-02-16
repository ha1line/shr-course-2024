#include <iostream>
#include <memory>

namespace math_const
{
    const double pi = 3.14;
}

// Figure
// P(), S()
class Figure
{
public:
    virtual ~Figure() = default;
    virtual double P() const = 0;
    virtual long double S() const = 0;
};

// Rectangle
class Rectangle : public Figure
{
public:
    Rectangle(int _w, int _h) :
        w(_w),
        h(_h)
    {
    }

    double P() const final
    {
        return 2 * (w + h);
    }

    long double S() const final
    {
        return w * h;
    }
private:
    int w;
    int h;
};

// Triangle
class Triangle : public Figure
{
public:
    Triangle(int _l1, int _l2, int _l3) :
        l1(_l1),
        l2(_l2),
        l3(_l3)
    {
    }

    double P() const final
    {
        return l1 + l2 + l3;
    }

    long double S() const final
    {
        return std::sqrt((P() / 2.0) * (P() / 2.0 - l1) * (P() / 2.0 - l2) * (P() / 2.0 - l3));;
    }

private:
    int l1;
    int l2;
    int l3;
};

// Circle
class Circle : public Figure
{
public:
    Circle(double _r) :
        r(_r)
    {
    }

    double P() const final
    {
        return 2 * r * math_const::pi;
    }

    long double S() const final
    {
        return math_const::pi * r * r;
    }
private:
    double r;
};

int main()
{
    std::vector<std::shared_ptr<Figure>> vec;
    // Заполняем вектор
    vec.push_back(std::make_shared<Triangle>(10, 5, 8));
    vec.push_back(std::make_shared<Circle>(10));
    vec.push_back(std::make_shared<Rectangle>(10, 5));
    for (const std::shared_ptr<Figure>& f : vec)
    {
        std::cout << f->P() << std::endl;
    }
}
