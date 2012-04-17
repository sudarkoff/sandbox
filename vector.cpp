#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{
    std::vector<char> v;
    v.insert(v.begin(), 'Z');
    v.push_back('9');

    std::copy(v.begin(), v.end(), std::ostream_iterator<char> (std::cout, "\n"));

    return 0;
}
