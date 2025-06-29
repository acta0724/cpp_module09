#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <limits>

std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Error: invalid number of arguments." << std::endl;
		return EXIT_FAILURE;
	}
	BitcoinExchange exchange;
	exchange.loadData("data.csv");
	std::ifstream file(argv[1]);
	if (!file.is_open()) {
		std::cerr << "Error: could not open file." << std::endl;
		return EXIT_FAILURE;
	}
	std::string line;
	std::getline(file, line);
	while (std::getline(file, line)) {
		if (line.empty()) {
			continue;
		}
		std::istringstream iss(line);
		std::string date;
		std::string valueStr;
		bool valid = true;
		if (std::getline(iss, date, '|') && std::getline(iss, valueStr)) {
			date = trim(date);
			valueStr = trim(valueStr);
			for (size_t i = 0; i < valueStr.size(); i++) {
				if (!std::isdigit(valueStr[i]) && !(valueStr[i] == '.') && !(valueStr[i] == '-' && i == 0)) {
					std::cerr << "Error: bad input => " << valueStr << std::endl;
					valid = false;
					break;
				}
			}
			double value = std::strtod(valueStr.c_str(), NULL);
			if (value < 0 && valid) {
				std::cerr << "Error: not a positive number." << std::endl;
				valid = false;
			}
			if (value > 1000 && valid) {
				std::cerr << "Error: too large a number." << std::endl;
				valid = false;
			}
			if (!valid) {
				continue;
			}
			double price;
			if (exchange.getPrice(date, price, value)) {
				std::cout << date << " => " << value << " = " << price << std::endl;
			} else {
				std::cerr << "Error: date not found in database." << std::endl;
			}
		} else {
			std::cerr << "Error: bad input => " << line << std::endl;
		}
	}
	return EXIT_SUCCESS;
}