#include <iostream>

int (&Foo())[5]
{
    static int goo[5] = {5, 4, 3, 2, 1};

    return goo;
}

int main()
{
    std::cout << Foo()[1];

    return 0;
}
