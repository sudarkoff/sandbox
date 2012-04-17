#include <iostream>

class A {
};

class B : public A {
};

int main()
{
	A* a = dynamic_cast<A*>(reinterpret_cast<B*>(0));
	std::cout << std::hex << a;
	return 0;
}
