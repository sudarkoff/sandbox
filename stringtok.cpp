#include <iostream>
#include <string>
#include <list>
#include "stringtok.h"

void main()
{
	std::list<std::string>  ls;
	stringtok (ls, "c:\\this\\is\\a\\path", "/\\");

	for (std::list<std::string>::const_iterator i = ls.begin(); i != ls.end(); ++i)
	{
		std::cerr << ':' << (*i) << ":\n";
	}
}