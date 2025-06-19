#pragma once

#include <map>
#include <string>

class BitcoinExchange
{
 private:
	std::map<std::string, double> _data;

 public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &src);
	BitcoinExchange &operator=(const BitcoinExchange &src);
	~BitcoinExchange();
	bool isValueValid(const std::string &value) const;
	void loadData(const std::string &filename);
	bool getPrice(const std::string &date, double &price, double amount) const;
};
