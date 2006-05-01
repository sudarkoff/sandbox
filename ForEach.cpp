#include <iostream>
#include <vector>
#include <algorithm>

class Sum {
public:
   int s;
   Sum() : s(0) {};
   void operator()(int i) { s+= i; }
};

int cdecl main(void)
{
   std::vector<int> v;

   std::cout << "Enter vector of integers followed by 0:" << std::endl;

   int i = 0;
   do {
      std::cin >> i;
      v.push_back(i);
   } while (i);

   Sum my_sum;
   // for_each(v.begin(),v.end(),my_sum);  // my_sum not changed!
   // for_each(v.begin(),v.end(),(Sum &)my_sum);  // my_sum not changed! (??)
   std::for_each<std::vector<int>::iterator, Sum&> (v.begin(), v.end(), my_sum);
   // now my_sum.s is sum of all element in v
   
   std::cout << my_sum.s;

   return 0;
}
