#include <iostream>
using std::cin;
using std::cout;
using std::endl;

inline long round(double value)
{
  return (value-(long)value)*10*((value < 0)? -1: 1) > 4 ? (long)value+((value < 0)? -1: 1) : (long)value;
}

int main(void)
{
  double number=0.0;

  cout << "Enter the number to round:" << endl;
  cin >> number;
  cout << number << " become " << round(number) << endl;
  
  return 0;
}