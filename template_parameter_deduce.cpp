#include <iostream>

template <typename T>
class DeduceType
{
public:
    typedef T template_argument_type;
    
    DeduceType(T const& argument)
    {
        std::cout << argument;
    }
};

int main()
{
    // this doesn't work
    DeduceType<> dt(123);
    return 0;
}
