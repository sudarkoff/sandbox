#include <iostream>

void strcat(char* dst, char* src)
{
	dst += strlen(dst);
	while(*dst++ = *src++);
}

int main()
{
    char dst[10] = "hell\0";
    char src[5] = "o";
	strcat(dst, src);
	std::cout << dst << std::endl;
}
