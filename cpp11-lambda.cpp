// New C++11 lambda syntax
#include <iostream>

int main()
{
    std::cout << "5^2 = " << [](int a) { return a*a; }(5) << std::endl;
    return 0;
}
