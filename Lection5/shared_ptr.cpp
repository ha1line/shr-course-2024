#include <iostream>
#include <list>
#include <memory>

struct Node;

struct RootNode
{
    ~RootNode()
    {
        std::cout << "~RootNode" << std::endl;
    }
    std::list<std::shared_ptr<Node>> m_widgets;
    // other fields
};


struct Node
{
    ~Node()
    {
        std::cout << "~Node" << std::endl;
    }
    std::shared_ptr<RootNode> rootWidget;
    // other fields
};

int main()
{
    // перекрестные ссылки
    {
        std::shared_ptr<RootNode> first = std::make_shared<RootNode>();
        std::shared_ptr<Node> second = std::make_shared<Node>();
        first->m_widgets.push_back(second);
        // у второго объекта use_count 2
        second->rootWidget = first;
        // у первого объекта use_count 2
    }
    // правильная инициализация std::shared_ptr
    {
        // плохая инициализация — 2 обращения к new
        std::shared_ptr<int> p(new int(5));
        // хорошая инициализация — 1 обращение к new
        std::shared_ptr<int> sp = std::make_shared<int>(5);
    }
}
