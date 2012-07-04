#include <iostream>
#include <time.h>

using namespace std;

int main(int argc, char* argv[])
{
  clock_t start, finish;

  start = clock();

  for (int i = 0; i < 1000000; ++i) {
	  int l = -i;
	  l = l - i;
  }
  
  finish = clock();

  cout << "usec per call: " << (int)( (double)(finish-start) / CLOCKS_PER_SEC * 1000) << endl;

  return 0;
}