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
	// int year = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day = std::atoi(date.substr(8, 2).c_str());
	if (month < 1 || month > 12 || day < 1 || day > 31) {
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
	// 日付の形式チェック
	if (!isValueValid(date)) {
		std::cerr << "Error: bad input => " << date << std::endl;
		return false;
	}

	// 完全一致する日付を検索
	std::map<std::string, double>::const_iterator it = _data.find(date);
	if (it != _data.end()) {
		price = it->second * amount;
		return true;
	}

	// 完全一致しない場合、その日付以前の最も近い日付を検索
	it = _data.lower_bound(date);
	if (it == _data.begin() && it->first > date) {
		// データベース内の最も古い日付よりも前の日付が指定された
		return false;
	}

	// lower_boundは指定した値以上の最初の要素を返すので、
	// 指定した日付より後の日付が返された場合は一つ前の要素を使用
	if (it == _data.end() || it->first > date) {
		--it;
	}

	price = it->second * amount;
	return true;
}
