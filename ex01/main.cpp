#include "RPN.hpp"
#include <iostream>

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Error: invalid number of arguments." << std::endl;
		return 1;
	}
	int result;
	if (!RPN::evaluate(argv[1], result)) {
		return 1;
	}
	std::cout << result << std::endl;
	return 0;
}