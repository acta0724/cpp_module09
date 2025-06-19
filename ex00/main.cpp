#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <limits>

// 文字列の前後の空白を削除する関数
std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Error: invalid number of arguments." << std::endl;
		return 1;
	}
	BitcoinExchange exchange;
	exchange.loadData("data.csv");
	std::ifstream file(argv[1]);
	if (!file.is_open()) {
		std::cerr << "Error: could not open file." << std::endl;
		return 1;
	}
	std::string line;
	std::getline(file, line);
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string date;
		std::string valueStr;
		if (std::getline(iss, date, '|') && std::getline(iss, valueStr)) {
			date = trim(date);
			valueStr = trim(valueStr);
			double value = std::strtod(valueStr.c_str(), NULL);
			if (value < 0) {
				std::cerr << "Error: not a positive number." << std::endl;
				continue;
			}
			if (value > 1000) {
				std::cerr << "Error: too large a number." << std::endl;
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
	return 0;
}