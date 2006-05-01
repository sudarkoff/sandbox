#include <iostream>
#include <bitset>

int main()
{
  int number;

  std::cout << "give number" << std::endl;
  std::cin >>  number;
  std::bitset<32> bits(number);
  std::cout << bits << std::endl;
  return 0;
}
