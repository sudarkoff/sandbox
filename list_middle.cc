#include <iostream>
#include <list>

typedef std::list<int> list_type;

list_type::iterator list_middle(list_type& l)
{
  list_type::iterator i_fast = l.begin(), i_slow = i_fast;
  for (size_t i = 0; i_fast != l.end(); ++i_fast, ++i) {
    if (i%2) ++i_slow;
  }

  return i_slow;
}

int main(int argc, char* argv[])
{
  list_type numbers;
  for (int i=0; i<10; ++i) {
    numbers.insert(numbers.end(), i);
  }

  list_type::iterator middle = list_middle(numbers);
  std::cout << *middle << std::endl;
  //remove_element(middle);
}
