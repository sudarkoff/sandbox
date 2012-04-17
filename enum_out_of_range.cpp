#include <iostream>

enum SomeType
{
    kOne = 1,
    kThree = 3
};

int main()
{
    SomeType someEnum;
    someEnum = static_cast<SomeType>(2);
    std::cout << someEnum;
}
