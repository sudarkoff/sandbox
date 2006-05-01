template<bool> struct CompileTimeChecker { CompileTimeChecker(...); };
template<> struct CompileTimeChecker<false> { };
#define STATIC_CHECK(expr, msg) \
	{ \
		class ERROR_##msg {}; \
		(void)sizeof(CompileTimeChecker<(expr)>(ERROR_##msg())); \
	}


int main(void)
{
	// This is OK
	STATIC_CHECK( sizeof(char) <= sizeof(int), Destination_Type_Too_Narrow );

	// This is not OK
	STATIC_CHECK( sizeof(double) <= sizeof(char), Destination_Type_Too_Narrow );

	return 0;
}
