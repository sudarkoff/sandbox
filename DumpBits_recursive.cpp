#include <iostream>

void DumpBits(int val, std::ostream& os)
{
  if (val != 0)
  {
    DumpBits(val/2, os);
    os << (val&1);
  }
}

int main()
{
  int number;

  std::cout << "give number" << std::endl;
  std::cin >>  number;
  if (number == 0)
  {
    std::cout << "0";
  }
  else
  {
    DumpBits(number, std::cout);
  }
  std::cout << std::endl;
  return 0;
}
