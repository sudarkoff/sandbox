/*
Simple calculator.

GRAMMAR
------- 
 expression = term {("+"|"-") term}
 term = factor {("*"|"/") factor}
 factor = number | unary-fu | binary-fu | "(" expression ")"
 unary-fu = ("sin"|"cos") "(" expression ")"
 binary-fu = ("min"|"max") "(" expression "," expression ")"

ASSUMPTIONS, LIMITATIONS, COMMENTS
----------------------------------
 - all tokens must be separated with a whitespace, e.g.: sin ( 0 ) + max ( 4 , 3 + 1 * 5 )
 - boost's lexical_cast is used to "parse" numbers
 - C++11 compiler is required (tested with g++ 4.7.0):
   $ g++ --std=c++11 virsto-calc.cpp && ./a.out test
 - run without any arguments to accept input from stdin
 - run with "test" as the first argument to execute a minimal set of unit tests
 - no effort was made to test negative cases - the program will probably crash on some of them
*/

#include <cctype>
#include <cmath>
#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <functional>
#include <stdexcept>
#include <boost/lexical_cast.hpp>

// A VERY hackish debugging macro
#ifdef __DEBUG
#  define trace(str) std::cout << "\t\t" << str
#else
#  define trace(str)
#endif

// Represents a single token
class Token {
public:
	// Types of tokens we support
	enum class TokenType { NUMBER, PLUS, MINUS, MULT, DIV, SIN, COS, MIN, MAX, COMMA, OPEN_BR, CLOSE_BR };

	Token(std::string const& tok, Token::TokenType type)
		: tok_(tok), type_(type)
	{
	}
    Token(Token const&) = default;
    Token& operator=(Token const&) = default;
	~Token() = default;

	// Compare Token to a TokenType
	bool operator==(TokenType type) { return (type_ == type); }
	// Return TokenType
	TokenType operator()() const { return type_; }
	// Return token string
	std::string const& str() const { return tok_; }

	// Token factory
	static Token make_token(std::string const& str)
	{
		// Special case for numbers - using boost's lexical_cast to parse
		try {
			boost::lexical_cast<double>(str);
			return Token(str, TokenType::NUMBER);
		}
		catch(boost::bad_lexical_cast&) {
			/* not a valid number, moving on */
		}

		// A table of all other tokens
		std::map<std::string, TokenType> tokType = {
			{ "+", TokenType::PLUS }, { "-", TokenType::MINUS }, { "*", TokenType::MULT }, { "/", TokenType::DIV },
			{ "sin", TokenType::SIN }, { "cos", TokenType::COS }, { "min", TokenType::MIN }, { "max", TokenType::MAX },
			{ ",", TokenType::COMMA }, { "(", TokenType::OPEN_BR }, { ")", TokenType::CLOSE_BR },
		};
		auto tok = tokType.find(str);
		if (tok != tokType.end()) {
			return Token(str, tok->second);
		}
		else {
			throw std::runtime_error("unknown token: " + str);
		}
	}

private:
	std::string tok_;
	TokenType type_;
};

// Implements a parser for the specified above grammar
class Parser {
public:
	Parser(std::string const& in)
	{
		tokenize(in);
		tok_ = tokens_.begin();
	}
	~Parser() = default;

	// Non-copyable
    Parser(Parser const&) = delete;
    Parser& operator=(Parser const&) = delete;

	// Parse the expression
	double parse()
	{
		trace("begin parsing..." << std::endl);
		expression();
		trace("done parsing" << std::endl);

		if (accumulator_.size() > 1) {
			throw std::runtime_error("too many results in the stack");
		}
		return accumulator_.top();
	}

protected:
	typedef std::function<double(double,double)> binary_fu_t;
	typedef std::function<double(double)> unary_fu_t;

	// Takes a string and turns it into a vector of tokens (tokes must be separated by a whitespace)
	void tokenize(std::string const& in, char const* const delimiters=" \t")
	{
		size_t len = in.length();
	    for (size_t i=0; i<len; )
	    {
	        // remove leading whitespace
	        i = in.find_first_not_of(delimiters, i);
	        if (i == std::string::npos) {
	            return;
			}

	        size_t j = in.find_first_of(delimiters, i);
	        if (j == std::string::npos) {
	            tokens_.push_back(Token::make_token(in.substr(i)));
	            return;
	        } else {
	            tokens_.push_back(Token::make_token(in.substr(i, j-i)));
			}

	        i = j + 1;
	    }
	}

	// Pop and remove one argument from the stack
	double stack_pop()
	{
		if (accumulator_.size() > 0) {
			double value = accumulator_.top();
			accumulator_.pop();
			trace("pop: stack size: " << accumulator_.size() << std::endl);
			return value;
		}
		else {
			throw std::runtime_error("operand stack is empty");
		}
	}

	// Push an argument to the stack
	void stack_push(double value)
	{
		accumulator_.push(value);
		trace("push: stack size: " << accumulator_.size() << std::endl);
	}

	// Returns true if there are no more tokens
	bool empty()
	{
		return (tok_ == tokens_.end());
	}

	// Returns a corresponding unary function for a given token
	unary_fu_t make_unary_fu(Token const& tok)
	{
		trace("make_unary_fu: token: " << tok.str() << std::endl);
		switch (tok()) {
		case Token::TokenType::SIN:
			return [](double op) { return std::sin(op); };
			break;
		case Token::TokenType::COS:
			return [](double op) { return std::cos(op); };
			break;
		default:
			throw std::runtime_error("unknown unary operation");
		}
	}

	// Returns a corresponding binary function for a given token
	binary_fu_t make_binary_fu(Token const& tok)
	{
		trace("make_binary_fu: token: " << tok.str() << std::endl);
		switch (tok()) {
		case Token::TokenType::PLUS:
			return [](double op1, double op2) { return (op1 + op2); };
			break;
		case Token::TokenType::MINUS:
			return [](double op1, double op2) { return (op1 - op2); };
			break;
		case Token::TokenType::MULT:
			return [](double op1, double op2) { return (op1 * op2); };
			break;
		case Token::TokenType::DIV:
			return [](double op1, double op2) { return (op1 / op2); };
			break;
		case Token::TokenType::MIN:
			return [](double op1, double op2) { return std::min(op1, op2); };
			break;
		case Token::TokenType::MAX:
			return [](double op1, double op2) { return std::max(op1, op2); };
			break;
		default:
			throw std::runtime_error("unknown binary operation");
		}
	}

	// The remaining methods implement the grammar

	void expression(void)
	{
		trace("expression: tok: " << tok_->str() << std::endl);
	    term();
	    while (!empty() && ((*tok_ == Token::TokenType::PLUS) || (*tok_ == Token::TokenType::MINUS))) {
			binary_fu_t op = make_binary_fu(*tok_);
			double op1 = stack_pop();
			trace("expression op1: " << op1 << std::endl);
	        ++tok_;
	        term();
			double op2 = stack_pop();
			trace("expression op2: " << op2 << std::endl);
			double result = op(op1, op2);
			trace("expression result: " << result << std::endl);
			stack_push(result);
	    }
	}

	void term(void)
	{
		trace("term: tok: " << tok_->str() << std::endl);
	    factor();
	    while (!empty() && ((*tok_ == Token::TokenType::MULT) || (*tok_ == Token::TokenType::DIV))) {
			binary_fu_t op = make_binary_fu(*tok_);
			double op1 = stack_pop();
	        ++tok_;
	        factor();
			double op2 = stack_pop();
			double result = op(op1, op2);
			trace("term result: " << result << std::endl);
			stack_push(result);
	    }
	}

	void factor(void)
	{
		trace("factor: tok: " << tok_->str() << std::endl);

	    if (*tok_ == Token::TokenType::NUMBER) {
			double number = boost::lexical_cast<double>(tok_->str());
	    	stack_push(number);
			trace("factor: " << number << std::endl);
			++tok_;
	    }
		else if ((*tok_ == Token::TokenType::SIN) || (*tok_ == Token::TokenType::COS)) {
			unary();
			++tok_;
		}
		else if ((*tok_ == Token::TokenType::MIN) || (*tok_ == Token::TokenType::MAX)) {
			binary();
			++tok_;
		}
		else if (!empty() && (*tok_ == Token::TokenType::OPEN_BR)) {
			++tok_;
	        expression();
	        if (!(*tok_ == Token::TokenType::CLOSE_BR)) {
	        	throw std::runtime_error("expected ')'");
	        }
		}
		else {
	        throw std::runtime_error("unexpected token: " + tok_->str());
	        ++tok_;
	    }
	}

	void unary(void)
	{
		trace("unary: tok: " << tok_->str() << std::endl);

		unary_fu_t op = make_unary_fu(*tok_);
        ++tok_;
		if (*tok_ == Token::TokenType::OPEN_BR) {
			++tok_;

	        expression();
	        if (!(*tok_ == Token::TokenType::CLOSE_BR)) {
	        	throw std::runtime_error("expected ')'");
	        }
			double op1 = stack_pop();

			double result = op(op1);
			trace("unary result: " << result << std::endl);
			stack_push(result);
		}
		else {
	        throw std::runtime_error("unexpected token: " + tok_->str());
	        ++tok_;
	    }
	}

	void binary(void)
	{
		trace("binary: tok: " << tok_->str() << std::endl);
		if (empty()) return;

		binary_fu_t op = make_binary_fu(*tok_);
		++tok_;
		if (*tok_ == Token::TokenType::OPEN_BR) {
			++tok_;

	        expression();
			double op1 = stack_pop();

			if (!(*tok_ == Token::TokenType::COMMA)) {
				throw std::runtime_error("binary operation is missing a second parameter");
			}
			++tok_;

			expression();
	        if (!(*tok_ == Token::TokenType::CLOSE_BR)) {
	        	throw std::runtime_error("expected ')'");
	        }
			double op2 = stack_pop();

			double result = op(op1, op2);
			trace("binary result: " << result << std::endl);
			stack_push(result);
		}
		else {
	        throw std::runtime_error("unexpected token: " + tok_->str());
	        ++tok_;
	    }
	}

private:
	std::vector<Token> tokens_;			// all tokens of the expression in order
	std::vector<Token>::iterator tok_;	// pointer to the currently evaluated token
	std::stack<double> accumulator_;	// operand stack
};

// Evaluate an expression and return the result
double eval(std::string const& expr)
{
	if (expr.length() > 0) { 
		Parser parser(expr);
		return parser.parse();
	}
	return 0.0;
}

// Compare expected and actual values, return a string containing the result and either OK or FAIL
std::string check_result(double got, double expected)
{
	const double EPSILON = 0.1E-10; // a fairly arbitrary number

	const char RED[] = "\x1b[31m";
	const char GREEN[] = "\x1b[32m";
	const char CLEAR[] = "\x1b[0m";

	std::stringstream str;
	if (fabs(got - expected) < EPSILON) {
		str << got << GREEN << " (OK) " << CLEAR;
	}
	else {
		str << got << RED << " (FAIL, expected: " << expected << ")" << CLEAR;
	}
	return str.str();
}

int main(int argc, char* argv[])
{
	// Run a few unit tests
	if ((argc > 1) && strncmp(argv[0], "test", 4)) {
		std::vector<std::pair<std::string, double>> expressions = {
			{ "", 0.0 },
			{ "0", 0.0 },
			{ "5", 5.0 },
			{ "-3", -3.0 },
			{ "3.14", 3.14 },
			{ "1 + 2", 3.0 },
			{ "5 - 10", -5.0 },
			{ "-1 + 3", 2.0 },
			{ "0 * 3", 0.0 },
			{ "2 * 2", 4.0 },
			{ "-1 * 3.0", -3.0 },
			{ "-10 * -3", 30.0 },
			{ "3.14 * 3", 9.42 },
			{ "99 * 3 + 1", 298.0 },
			{ "1 + 3 * 99", 298.0 },
			{ "99 * ( 3 + 1 )", 396.0 },
			{ "sin ( -3 + 3 ) + 1", 1.0 },
			{ "cos ( 4 - 4 )", 1.0 },
			{ "cos ( sin ( -3 + 3.0 ) )", 1.0},
			{ "min ( -3 , 3 )", -3.0 },
			{ "max ( -3 , 3 )", 3.0 },
			{ "max ( min ( -3 , 3 ) , cos ( 4 - 4 ) )",  1.0 }
		};
		for (auto& expr : expressions) {
			try {
				double result = eval(expr.first);
				std::cout << expr.first << " = " << check_result(result, expr.second) << std::endl;
			}
			catch (std::runtime_error& err) {
				std::cerr << err.what() << std::endl;
			}
		}
	}
	// ... or read expressions from std::cin
	else {
		std::string expr;
		while (std::cin) {
			getline(std::cin, expr);
			try {
				double result = eval(expr);
				std::cout << "\t= " << result << std::endl;
			}
			catch (std::runtime_error& err) {
				std::cerr << err.what() << std::endl;
			}
		}
	}
}
