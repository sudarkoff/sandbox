#include <iostream>

class Overflow {
unsigned int flag;

public:
	Overflow() 
		{ flag=10; 
		  std::cout << "{{{ " << flag << ":Overflow::Overflow()\n"; 
		}

	Overflow(int f)
		{ flag=f; 
		  std::cout << "{{{ " << flag << ":Overflow::Overflow(int)\n"; 
		}

	Overflow(const Overflow& o) 
		{ flag=o.flag+1; 
		  std::cout << "{{{ " << flag << ":Overflow::Overflow(const Overflow&)\n"; 
		}

	virtual ~Overflow() 
		{ std::cout << "}}} " << flag << ":Overflow::~Overflow()\n"; }

	virtual void vf() { std::cout << "\t" << flag << ":Overflow::vf()\n"; }
};

void foo()
{
	throw Overflow(0);
}

int main(void)
{
	try {
		foo();
	}
	catch(Overflow& o) {
		o.vf();
	}
	return 0;
}
