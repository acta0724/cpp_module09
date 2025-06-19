#include "RPN.hpp"
#include <stack>
#include <sstream>
#include <iostream>

RPN::RPN() {}

RPN::~RPN() {}

RPN::RPN(const RPN &src) {
	(void)src;
}

RPN &RPN::operator=(const RPN &src) {
	if (this != &src) {
		(void)src;
	}
	return *this;
}

bool RPN::evaluate(const std::string &expression, int &result) {
	std::stack<int> stack;
	std::istringstream iss(expression);
	std::string token;
	int a, b;

	while (iss >> token) {
		if (token.size() == 1 && std::isdigit(token[0])) {
			stack.push(token[0] - '0');
		} else if (token == "+" || token == "-" || token == "*" || token == "/") {
			if (stack.size() < 2) {
				std::cerr << "Error: invalid expression." << std::endl;
				return false;
			}
			b = stack.top();
			stack.pop();
			a = stack.top();
			stack.pop();
			if (token == "+") {
				stack.push(a + b);
			} else if (token == "-") {
				stack.push(a - b);
			} else if (token == "*") {
				stack.push(a * b);
			} else if (token == "/") {
				if (b == 0) {
					std::cerr << "Error: division by zero." << std::endl;
					return false;
				}
				stack.push(a / b);
			}
		}
	}
	if (stack.size() != 1) {
		std::cerr << "Error: invalid expression." << std::endl;
		return false;
	}
	result = stack.top();
	return true;
}
