#include <functional>
#include <iostream>

// A base class for the callback
struct Callback {};

// Specification of the callback method
typedef std::function<void(Callback&, int)> f_callback_t;

// A function that takes a callback as a parameter
void Foo(int N, f_callback_t& callback) {
   for (int i=0; i<N; i++) {
      callback(i);
   }
}

// Our callback
struct Sum : public Callback {
   void operator() (int num) { num_ += num; }
   int sum() { return num_; }

   int num_;
};

int main()
{
   Sum sum;
   Foo(100, sum);
   std::cout << sum.sum();

   return 0;
}

