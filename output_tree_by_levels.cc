#include <iostream>

int main(int argc, char* argv)
{
  if (argc < 2) {
    std::cout << "Pass the tree file..."
  }

  node* root = read_lrep(std::ifstream(argv[1]));

}
