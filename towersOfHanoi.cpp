#include <iostream>

int main()
{
	int n = 3;
	for (int x=1; x < (1 << n); x++) {
		std::cout << "Move from pole " << (x&x-1)%3 << " to pole " << (((x|x-1)+1)%3) << std::endl;
	}
}
