#include <iostream>

class Base {};
class Derived : public Base {};

int main(int argc, char* argv[])
{
   try {
      throw Derived();
   } catch (Base&) {
      std::cout << "caught Base!\n";
   }

   return 0;
}

