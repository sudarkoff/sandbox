#include <stdlib.h>

void main(void)
{
	char *str = "This is a test string",
		 ch;
	long i = 0,
		 max = strlen(str)-1;

	while ((max/2 - i) >= 0)
	{
		ch = str[i];
		str[i] = str[max-i];
		str[max-i] = ch;
		i++;
	}

	printf("%s", str);
}
