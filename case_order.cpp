#include <iostream>

int main()
{
    int a = 2;
    switch (a)
    {
        case 1:
            std::cout << "1" << std::endl;
            break;
        case 2:
            std::cout << "2" << std::endl;
            break;
        case 3:
            std::cout << "3" << std::endl;
            break;
        default:
            std::cout << "default" << std::endl;
            break;
    }
    return 0;
}
