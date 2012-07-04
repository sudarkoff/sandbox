#include <iostream>

template <bool b, int inhibit_inst = 0>
struct executor;

template <int inhibit_inst>
struct executor<true, inhibit_inst>
{
   static void foo() { std::cout << "compile if true\n"; }
};

template <int inhibit_inst>
struct executor<false, inhibit_inst>
{
   static void foo() { std::cout << "compile if false\n"; }
};


template <bool b>
inline void execute()
{
	return executor<b>::foo();
}

int main()
{
   execute<(1==1)>();
}
