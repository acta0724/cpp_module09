#include "RPN.hpp"
#include <iostream>

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Error: invalid number of arguments." << std::endl;
		return EXIT_FAILURE;
	}
	int result;
	if (!RPN::evaluate(argv[1], result)) {
		return EXIT_FAILURE;
	}
	std::cout << result << std::endl;
	return EXIT_SUCCESS;
}