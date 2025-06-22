#pragma once

#include <vector>
#include <list>

class PmergeMe {
private:
	PmergeMe();
	PmergeMe(const PmergeMe &src);
	PmergeMe &operator=(const PmergeMe &src);
	~PmergeMe();

	static size_t nextJacobsthal(size_t n);

public:
	// Ford-Johnson（マージ挿入ソート）アルゴリズムを使用してベクトルをソート
	template <typename T>
	static std::vector<T> fordJohnsonVec(std::vector<T>& arr);
	
	// リストをソート
	// static void sortList(std::list<int> &lst);
	template <typename T>
	static std::list<T> fordJohnsonList(std::list<T> &lst);
};
