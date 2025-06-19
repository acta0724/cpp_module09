#pragma once

#include <string>
#include <stack>

class RPN {
private:
	RPN();
	RPN(const RPN &src);
public:
	RPN &operator=(const RPN &src);
	~RPN();
	static bool evaluate(const std::string &expression, int &result);
};
