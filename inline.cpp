class A
{
public:
	void bar();
};

void foo()
{
	A a;
	a.bar();
}

inline void A::bar()
{
}

int main()
{
	return 0;
}
