#include "BitcoinExchange.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &src) {
	_data = src._data;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &src) {
	if (this != &src) {
		_data = src._data;
	}
	return *this;
}

BitcoinExchange::~BitcoinExchange() {}

bool BitcoinExchange::isValueValid(const std::string &date) const {
	if (date.empty()) {
		return false;
	}
	if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
		return false;
	}
	for (size_t i = 0; i < date.size(); i++) {
		if (i == 4 || i == 7) {
			continue;
		}
		if (!std::isdigit(date[i])) {
			return false;
		}
	}
	int year = std::stoi(date.substr(0, 4));
	int month = std::stoi(date.substr(5, 2));
	int day = std::stoi(date.substr(8, 2));
	if (month < 1 || month > 12 || day < 1) {
		return false;
	}
	int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
		daysInMonth[2] = 29;
	}
	if (day > daysInMonth[month]) {
		return false;
	}
	
	return true;
}

void BitcoinExchange::loadData(const std::string &filename) {
	std::ifstream file(filename.c_str());
	if (!file.is_open()) {
		std::cerr << "Error: could not open file." << std::endl;
		std::exit(1);
	}
	std::string line;
	std::getline(file, line);
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string date;
		double value;
		if (std::getline(iss, date, ',') && iss >> value && isValueValid(date)) {
			_data[date] = value;
		} else {
			std::cerr << "Error: invalid data." << std::endl;
			std::exit(1);
		}
	}
}

bool BitcoinExchange::getPrice(const std::string &date, double &price, double amount) const {
	if (!isValueValid(date)) {
		std::cerr << "Error: bad input => " << date << std::endl;
		return false;
	}

	std::map<std::string, double>::const_iterator it = _data.find(date);
	if (it != _data.end()) {
		price = it->second * amount;
		return true;
	}

	it = _data.lower_bound(date);
	if (it == _data.begin() && it->first > date) {
		return false;
	}

	if (it == _data.end() || it->first > date) {
		--it;
	}

	price = it->second * amount;
	return true;
}
