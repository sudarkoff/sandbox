#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>

/*
 * expression := number | number + expression
 * number := digit | digit digit0*
 * digit := 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
 * digit0 := digit | 0
 */

int Eval(std::string expression)
{
	// illegal expressions
    if ( expression.length() == 0 ||  // empty expression
         expression == "0" ||         // zero
         expression[0] == '0' ||      // leading zero
         !std::isdigit(expression[0]) // first char is not a digit
       ) {
        return -1;
    }
    
    int sum = 0;
    for (size_t i = 0; i <= expression.length(); ++i) {
		std::string item = "";
		while ((i < expression.length()) && std::isdigit(expression[i])) {
			item += expression[i++];
		}
		if (((expression[i] == '+') && (i < expression.length() - 1)) || (i == expression.length())) {
    		sum += boost::lexical_cast<int>(item); // should not throw by way of constructing item
		}
		else {
			return -1;
		}
    }
    
    std::cerr << sum << std::endl;
	return sum;
}

int main()
{
    // bad expressions
	assert(Eval("") == -1);
	assert(Eval("0") == -1); // according to syntax diagram
  	assert(Eval("01") == -1);
  	assert(Eval("+") == -1);
  	assert(Eval("5+") == -1);
  	assert(Eval("5+10+") == -1);
	assert(Eval("34-4") == -1);

    // correct expressions
	assert(Eval("1+2") == 3);
	assert(Eval("99+3+1") == 103);
	assert(Eval("5") == 5);
}
