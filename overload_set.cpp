/*
 * Overloaded lambda expressions.
 *
 * From this article:
 * http://cpp-next.com/archive/2012/09/unifying-generic-functions-and-function-objects/
 *
 * Build:
 *    $ g++ --std=c++11 overload_set.cpp && ./a.out
 *    (FIXME: I actually don't have c++11 compiler on my system right now, so this is not tested.)
 */
template <class F1, class F2>
struct overload_set : F1, F2
{
   overload_set(F1 x1, F2 x2) : F1(x1), F2(x2) {}
   using F1::operator();
   using F2::operator();
};
 
template <class F1, class F2>
overload_set<F1,F2> overload(F1 x1, F2 x2)
{
   return overload_set<F1,F2>(x1,x2);
}
 
int main(int argc, const char *argv[])
{
   auto f = overload(
      []() { return 1; }, 
      [](int x) { return x+1; }
   );
 
   int x = f();
   int y = f(2);
   
   return 0;
}
