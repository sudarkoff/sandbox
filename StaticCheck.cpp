#define STATIC_ASSERT(expr) { char unnamed[(expr)? 1: 0]; }

int main(void)
{
	STATIC_ASSERT (false);
	return 0;
}
