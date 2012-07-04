#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

int main() 
{
	std::string text("lower case");
	std::cout << text << std::endl;
	std::transform(text.begin(), text.end(), text.begin(), std::toupper);
	std::cout << text << std::endl;
	
	return 0;
}