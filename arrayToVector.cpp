#include <iostream>
#include <vector>
#include <iterator>

int main(int, char*[])
{
    int a[5] = { 1, 2, 3, 4, 5 };
    
    // initialize the vector with array data
    std::vector<int> v1(a, a+5);
    copy(v1.begin(), v1.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    
    // copy data to the existing vector
    std::vector<int> v2;
    copy(a, a+5, std::inserter(v2, v2.begin()));
    copy(v2.begin(), v2.end(), std::ostream_iterator<int>(std::cout, " "));
    
    return 0;
}
