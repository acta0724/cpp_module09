#include "PmergeMe.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>
#include <climits>
#include <cerrno>
#include <iomanip>

bool isPositiveInteger(const std::string& str) {
    if (str.empty()) 
		return false;
    for (size_t i = 0; i < str.length(); ++i) {
        if (!std::isdigit(str[i])) 
			return false;
    }
    return true;
}

bool stringToInt(const std::string& str, int& result) {
    errno = 0;
    char* endptr;
    long value = std::strtol(str.c_str(), &endptr, 10);
    if (errno == ERANGE || *endptr != '\0' || str.empty() || value <= 0 || value > INT_MAX) {
        return false;
    }
    result = static_cast<int>(value);
    return true;
}

int main(int argc, char **argv) {
	std::vector<int> vec;
    std::vector<int> originalVec;
    std::list<int> lst;
    std::string token;

	if (argc != 2) {
		std::cerr << "Error: invalid number of arguments." << std::endl;
		return EXIT_FAILURE;
    }
    std::istringstream iss(argv[1]);
	int value;
    while (iss >> token) {
        if (!isPositiveInteger(token) || !stringToInt(token, value)) {
            std::cerr << "Error: invalid argument '" << token << "'. Only positive integers are allowed." << std::endl;
            return EXIT_FAILURE;
        }
        vec.push_back(value);
        originalVec.push_back(value);
        lst.push_back(value);
    }
    if (vec.empty()) {
        std::cerr << "Error: no valid integers provided." << std::endl;
        return EXIT_FAILURE;
    }
    clock_t start = clock();
    vec = PmergeMe::fordJohnsonVec(vec);
    clock_t end = clock();
    double time = (double)(end - start) / CLOCKS_PER_SEC * 1e6;
    
    std::cout << "Before: ";
    for (size_t i = 0; i < originalVec.size() && i < 10; ++i) {
        std::cout << originalVec[i] << " ";
    }
    if (originalVec.size() > 10)
		std::cout << "[...]";
    std::cout << std::endl;
    
    std::cout << "After: ";
    for (size_t i = 0; i < vec.size() && i < 10; ++i) {
        std::cout << vec[i] << " ";
    }
    if (vec.size() > 10)
		std::cout << "[...]";
    std::cout << std::endl;
    
    std::cout << "Time to process a range of " << vec.size() << " elements with std::vector : " 
              << std::fixed << std::setprecision(5) << time / 1000000 << " us" << std::endl;
    
    start = clock();
    lst = PmergeMe::fordJohnsonList(lst);
    end = clock();
    time = (double)(end - start) / CLOCKS_PER_SEC * 1e6;
    std::cout << "Time to process a range of " << lst.size() << " elements with std::list : " 
              << std::fixed << std::setprecision(5) << time / 1000000 << " us" << std::endl;
    return EXIT_SUCCESS;
}
