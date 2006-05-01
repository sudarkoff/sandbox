#include <iostream>

void foo() throw(int)
{
  throw 10;
}

int main(int argc, char* argv[])
{
  try {
    foo();
  }
  catch (int i) {
    std::cout << "int: " << i << std::endl;
  }
}
