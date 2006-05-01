template<bool> struct CompileTimeError;
template<> struct CompileTimeError<true> {};
#define STATIC_CHECK(expr) (CompileTimeError<(expr)!=0>())

int main(void)
{
	STATIC_CHECK(false);
	return 0;
}