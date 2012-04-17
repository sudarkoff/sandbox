#include <iostream>
#include "list.h"

// recursive version (good for relatively short lists)
test::node* reverse1(test::node* curr)
{
  test::node* tail = curr;
  test::node* next = curr->next_;

  if (next) {
    tail = reverse1(next);
    next->next_ = curr;
    curr->next_ = 0;
  }

  return tail;
}

// while-loop version (good for lists of any length)
test::node* reverse2(test::node* curr)
{
  test::node* prev = 0;
  test::node* next = 0;

  while (curr) {
    next = curr->next_;
    curr->next_ = prev;
    prev = curr;
    curr = next;
  }

  return prev;
}

int main(int argc, char* argv[])
{
  // create the list
  test::node* List = new test::node('a');
  for (char i = 'b'; i <= 'g'; ++i) {
    List->push_back(i);
  }
  List->print();

  // reverse the list recursively
  List = reverse1(List);
  List->print();
  
  // reverse the list again using while loop
  List = reverse2(List);
  List->print();

  // destroy the list
  delete List;

  return 0;
}
