#include <fstream>
#include <vector>
#include <algorithm>

int cdecl main(void)
{
	std::vector< std::pair<long, long> > dieList;

	std::ifstream fstrm("dielist");
	while (!fstrm.eof()) {
		long x, y;
		fstrm >> x >> y;
		std::pair<long, long> die(x,y);
		dieList.push_back(die);
	}

	return 0;
}
