#include <iostream>
#include <string>

int main()
{
	std::string test("short string");
	std::cout << test.substr(test.size(), 10);
	
	return 0;
}
