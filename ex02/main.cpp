#include "PmergeMe.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <limits>
#include <cerrno>
#include <iomanip>
#include <sys/time.h>

unsigned long getTimeInMicroseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

bool isPositiveInteger(const std::string& str) {
    if (str.empty()) 
		return false;
    
    for (size_t i = 0; i < str.length(); ++i) {
        if (!std::isdigit(str[i])) 
			return false;
    }
    
    return true;
}

bool stringToInt(const std::string& str, int& value) {
    std::istringstream iss(str);
    iss >> value;
    return !iss.fail() && iss.eof() && value >= 0;
}

int main(int argc, char **argv) {
    try {
        if (argc < 2) {
            std::cerr << "Error: invalid number of arguments." << std::endl;
            return EXIT_FAILURE;
        }
        
        std::vector<int> vec;
        std::list<int> lst;
        
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            std::istringstream iss(arg);
            std::string token;
            
            while (iss >> token) {
                int value;
                if (!isPositiveInteger(token) || !stringToInt(token, value)) {
                    std::cerr << "Error: invalid argument '" << token << "'. Only positive integers are allowed." << std::endl;
                    return EXIT_FAILURE;
                }
                
                for (size_t j = 0; j < vec.size(); ++j) {
                    if (vec[j] == value) {
                        std::cerr << "Error: duplicate value '" << value << "' found. All values must be unique." << std::endl;
                        return EXIT_FAILURE;
                    }
                }
                
                vec.push_back(value);
                lst.push_back(value);
            }
        }
        
        if (vec.empty()) {
            std::cerr << "Error: no valid integers specified." << std::endl;
            return EXIT_FAILURE;
        }
        
        std::cout << "Before vector: ";
        for (size_t i = 0; i < vec.size() && i < 10; ++i) {
            std::cout << vec[i] << " ";
        }
        if (vec.size() > 10) {
            std::cout << "[...]";
        }
        std::cout << std::endl;
        
        unsigned long startVec = getTimeInMicroseconds();
        std::vector<int> sortedVec = PmergeMe::fordJohnsonVec(vec);
        unsigned long endVec = getTimeInMicroseconds();
        unsigned long vecTime = endVec - startVec;
        size_t vecComparisons = PmergeMe::getVecComparisonCount();
        
        unsigned long startList = getTimeInMicroseconds();
        std::list<int> sortedList = PmergeMe::fordJohnsonList(lst);
        unsigned long endList = getTimeInMicroseconds();
        unsigned long listTime = endList - startList;
        size_t listComparisons = PmergeMe::getListComparisonCount();
        
        std::cout << "After vector:   ";
        for (size_t i = 0; i < sortedVec.size(); ++i) {
            std::cout << sortedVec[i] << " ";
        }
        if (sortedVec.size() > 10) {
            std::cout << "[...]";
        }
        std::cout << std::endl;

		std::cout << "corrent sorted: ";
		for (size_t i = 1; i < vec.size() + 1; ++i) {
			std::cout << i << " ";
		}
		std::cout << std::endl;

		std::cout << "After list:     ";
        std::list<int>::iterator it = sortedList.begin();
        for (size_t i = 0; i < sortedList.size(); ++i) {
            if (it != sortedList.end()) {
                std::cout << *it << " ";
                ++it;
            }
        }
        if (sortedList.size() > 10) {
            std::cout << "[...]";
        }
        std::cout << std::endl;
        
        bool isSortedVec = std::is_sorted(sortedVec.begin(), sortedVec.end());
		bool isSortedList = std::is_sorted(sortedList.begin(), sortedList.end());
        if (!isSortedVec || !isSortedList) {
            std::cerr << "Error: sorting failed." << std::endl;
            return EXIT_FAILURE;
        } else {
            std::cout << "Sorting completed successfully." << std::endl;
        }
        
        std::cout << "Time to process a range of " 
                  << std::setw(5) 
                  << std::setfill(' ') << vec.size()
                  << " elements with std::vector : "
                  << vecTime << " us" 
                  << "(" << vecComparisons << " comparisons)" << std::endl;
        
        std::cout << "Time to process a range of " 
                  << std::setw(5) 
                  << std::setfill(' ') << lst.size()
                  << " elements with std::list : "
                  << listTime << " us"
                  << "(" << listComparisons << " comparisons)" << std::endl;
        
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Error: unknown error occurred." << std::endl;
        return EXIT_FAILURE;
    }
}
