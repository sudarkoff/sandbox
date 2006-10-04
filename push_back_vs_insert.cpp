#include <iostream>
#include <list>

int main()
{
    std::list<double> l1;
    std::list<double> l2;

    time_t t1_begin = time(NULL);
    for (size_t i = 0; i < 10000000; ++i)
    {
        l1.push_back(i * 3.1415926);
    }
    time_t t1_end = time(NULL);
    std::cout << "push_back(...): " << difftime(t1_end, t1_begin) << std::endl;
    
    time_t t2_begin = time(NULL);
    for (size_t i = 0; i < 10000000; ++i)
    {
        l2.insert(l2.end(), i * 3.1415926);
    }
    time_t t2_end = time(NULL);
    std::cout << "insert(end(), ...): " << difftime(t2_end, t2_begin) << std::endl;
}
