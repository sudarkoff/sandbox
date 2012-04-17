#include <iostream>
#include <time.h>

using namespace std;

int cdecl main(void)
{
  clock_t start, finish;

  start = clock();

  //...
  
  finish = clock();

  cout << "usec per call: " << (int)( (double)(finish-start) / CLOCKS_PER_SEC * 1000) << endl;

  return 0;
}