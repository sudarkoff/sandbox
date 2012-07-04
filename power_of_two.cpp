#include <iostream>

int main()
{
	long number = 32;
	if ((number > 0) && ((number & (number - 1)) == 0)) {
		std::cout << "Given number is the power of two" << std::endl;
	}
	return 0;
}
