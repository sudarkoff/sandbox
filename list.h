#include <iostream>

namespace test {

  struct node {
    char value_;
    node* next_;

    // add the node to the end of the list
    void push_back(char c)
    {
      node* curr = this;
      while (curr->next_) curr = curr->next_;
      curr->next_ = new node(c);
    }

    // print the node and all its siblings
    void print()
    {
      node* curr = this;
      do {
	std::cout << curr->value_ << " ";
	curr = curr->next_;
      } while (curr);
      std::cout << std::endl;
    }

    node() : value_(0), next_(0) {}
    node(char c) : value_(c), next_(0) {}
    virtual ~node() { delete next_; }
  };

} // namespace test
