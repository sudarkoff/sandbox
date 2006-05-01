#include <fstream>
#include <string>

int main()
{
/*1*/
	std::ifstream fstrm("C:"\dummy.txt");
	std::istreambuf_iterator<char> beg(fstrm), end;
	std::string str(beg, end);

/*2*/
	std::ifstream fstrm("C:"\dummy.txt");
	std::istreambuf_iterator<char> beg(fstrm), end;
	std::string str;
	std::copy(beg, end, std::inserter(str, str.end()));

/*3 Or much faster: */
	std::ifstream fstrm("C:"\dummy.txt");

	//seek to end
	fstrm.seekg(0, std::ios_base::end);
	std::streampos size = fstrm.tellg();
	//rewind again
	fstrm.seekg(0);

	std::istreambuf_iterator<char> beg(fstrm), end;
	std::string str(size); //hope size isn't too big else undefined behaviour
	std::copy(beg, end, str.begin());

/*4*/
	std::getline (static_cast <std::istream&> (fstrm), str, (char) EOF);

    return 0;
}
