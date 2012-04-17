#include <string>
#include <algorithm>
#include <iostream>

int main()
{
	std::string str = "This is a test string.";
	std::reverse(str.begin(), str.end());
	std::cout << str << std::endl;

	return 0;
}