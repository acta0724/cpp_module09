#include "PmergeMe.hpp"
#include <algorithm>
#include <utility>

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe &src) {
	(void)src;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &src) {
	if (this != &src) {
		(void)src;
	}
	return *this;
}

PmergeMe::~PmergeMe() {}

// テンプレート関数の実装
template <typename T>
std::list<T> PmergeMe::fordJohnsonList(std::list<T> &arr) {
    if (arr.size() <= 1) return arr;  // base case

    // ① pairing
    std::list<T> winners;
    std::list<T> losers;
    
    typename std::list<T>::iterator it = arr.begin();
    while (it != arr.end()) {
        typename std::list<T>::iterator next = it;
        ++next;
        
        if (next != arr.end()) {
            // ペアの比較
            if (*it < *next) {
                winners.push_back(*next);
                losers.push_back(*it);
            } else {
                winners.push_back(*it);
                losers.push_back(*next);
            }
            it = ++next;  // 次のペアに進む
        } else {
            // 奇数要素の処理
            winners.push_back(*it);
            ++it;
        }
    }

    // ② recursive sort of winners
    winners = fordJohnsonList(winners);

    // ③ insert first loser at front
    if (!losers.empty()) {
        T firstLoser = losers.front();
        losers.pop_front();
        winners.push_front(firstLoser);
    }

    // ④ generate Jacobsthal sequence indices
    std::list<size_t> J;  // 1,3,4,6,7,...
    for (size_t k = 1; J.empty() || J.back() < losers.size(); ++k) {
        size_t j = nextJacobsthal(k);
        if (j >= 1) {
            size_t idx = std::min(j, losers.size());
            J.push_back(idx);
        }
    }

    // ⑤ binary insert losers in J-order
    for (typename std::list<size_t>::iterator jIt = J.begin(); jIt != J.end(); ++jIt) {
        size_t idx = *jIt;
        if (idx == 0 || idx > losers.size()) continue;

        // idx番目の要素を取得
        typename std::list<T>::iterator loserIt = losers.begin();
        std::advance(loserIt, idx - 1);
        T val = *loserIt;
        losers.erase(loserIt);

        // 二分探索で挿入位置を見つける
        typename std::list<T>::iterator insertPos = winners.begin();
        size_t left = 0;
        size_t right = idx;

        while (left < right) {
            size_t mid = left + (right - left) / 2;
            typename std::list<T>::iterator midIt = winners.begin();
            std::advance(midIt, mid);

            if (*midIt < val) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }

        // 挿入位置に移動
        insertPos = winners.begin();
        std::advance(insertPos, left);
        winners.insert(insertPos, val);
    }

    // ⑥ return
    return winners;
}

// テンプレート関数の実装
template <typename T>
std::vector<T> PmergeMe::fordJohnsonVec(std::vector<T>& arr) {
    // Base case: if array has 0 or 1 element, it's already sorted
    if (arr.size() <= 1) return arr;
    
    // Use merge-insertion sort (Ford-Johnson algorithm)
    std::vector<std::pair<T, T> > pairs;
    size_t i;
    
    // Create pairs
    for (i = 0; i + 1 < arr.size(); i += 2) {
        if (arr[i] > arr[i + 1])
            pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
        else
            pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
    }
    
    // Sort pairs by their larger element
    std::vector<T> result;
    std::vector<T> pending;
    
    // Sort pairs using insertion sort
    for (size_t j = 1; j < pairs.size(); ++j) {
        std::pair<T, T> key = pairs[j];
        int k = j - 1;
        while (k >= 0 && pairs[k].first > key.first) {
            pairs[k + 1] = pairs[k];
            --k;
        }
        pairs[k + 1] = key;
    }
    
    // Extract sorted larger elements
    for (size_t j = 0; j < pairs.size(); ++j) {
        result.push_back(pairs[j].first);
        pending.push_back(pairs[j].second);
    }
    
    // Handle odd element if present
    if (i < arr.size()) {
        // Binary search to find insertion point for odd element
        T oddElement = arr[i];
        size_t left = 0;
        size_t right = result.size();
        
        while (left < right) {
            size_t mid = left + (right - left) / 2;
            if (result[mid] < oddElement)
                left = mid + 1;
            else
                right = mid;
        }
        
        result.insert(result.begin() + left, oddElement);
    }
    
    // Insert smaller elements using binary search
    for (size_t j = 0; j < pending.size(); ++j) {
        T element = pending[j];
        size_t left = 0;
        size_t right = result.size();
        
        while (left < right) {
            size_t mid = left + (right - left) / 2;
            if (result[mid] < element)
                left = mid + 1;
            else
                right = mid;
        }
        
        result.insert(result.begin() + left, element);
    }
    
    return result;
}

// 明示的なテンプレートのインスタンス化
template std::vector<int> PmergeMe::fordJohnsonVec(std::vector<int>& arr);
template std::list<int> PmergeMe::fordJohnsonList(std::list<int>& lst);

// --- utils ---------------------------------------------
size_t PmergeMe::nextJacobsthal(size_t n) {          // J(n)
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    // Use iterative approach to avoid stack overflow
    size_t a = 0;
    size_t b = 1;
    size_t result = 0;
    
    for (size_t i = 2; i <= n; ++i) {
        result = b + 2 * a;
        a = b;
        b = result;
    }
    
    return result;
}
