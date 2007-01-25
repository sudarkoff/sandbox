#include <iostream>

int main()
{
	std::cout << "How many disks?" << std::endl;
	int n;
	std::cin >> n;
	for (int x=1; x < (1 << n); x++) {
		std::cout << "Move from pole " << (x&x-1)%3 << " to pole " << (((x|x-1)+1)%3) << std::endl;
	}
}
