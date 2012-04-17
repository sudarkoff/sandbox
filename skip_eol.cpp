#include <fstream>
#include <limits>

int main(void)
{
   std::ifstream ifstrm("file.txt");
   ifstrm.ignore(std::numerical_limits::max<int>(), '\n');

   return 0;
}
