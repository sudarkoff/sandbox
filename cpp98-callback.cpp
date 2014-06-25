#include <iostream>

// Base class for the callback
class Callback {
   public:
      virtual ~Callback() {}

      virtual bool operator()(int) { return true; }
};

// Our concrete callback that sums up the numbers passed to it
class Sum : public Callback {
   public:
      Sum() : num_(0) {}
      virtual ~Sum() {}

      bool operator()(int num) { num_ += num; return true; }
      int sum() const { return num_; }

   private:
      long long int num_;
};

class Mult : public Callback {
   public:
      Mult() : num_(1) {}
      virtual ~Mult() {}

      bool operator()(int num) { num_ *= num; return true; }
      int mult() const { return num_; }

   private:
      long long int num_;
};

bool foo(int bar, Callback* callback)
{
   for (int i=1; i<=bar; i++) {
      (*callback)(i);
   }
   return true;
}

int main()
{
   Sum sum;
   foo(10, &sum);
   std::cout << "sum(10): " << sum.sum() << std::endl;

   Mult mult;
   foo(10, &mult);
   std::cout << "mult(10): " << mult.mult() << std::endl;

   return 0;
}

