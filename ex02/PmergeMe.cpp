#include "PmergeMe.hpp"
#include <algorithm>
#include <utility>

size_t g_vecComparisonCount = 0;
size_t g_listComparisonCount = 0;

size_t PmergeMe::vecComparisonCount = 0;
size_t PmergeMe::listComparisonCount = 0;

void PmergeMe::resetComparisonCounts() {
    vecComparisonCount = 0;
    listComparisonCount = 0;
    g_vecComparisonCount = 0;
    g_listComparisonCount = 0;
}

size_t PmergeMe::getVecComparisonCount() {
    return g_vecComparisonCount;
}
size_t PmergeMe::getListComparisonCount() {
    return g_listComparisonCount;
}

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

template<typename Iterator, typename T>
Iterator optimizedLowerBound(Iterator first, Iterator last, const T& val, size_t& compCount) {
    while (first != last) {
        Iterator mid = first + std::distance(first, last) / 2;
        ++compCount;
        if (*mid < val) {
            first = mid + 1;
        } else {
            last = mid;
        }
    }
    return first;
}

template <typename T>
std::vector<T> PmergeMe::fordJohnsonVec(std::vector<T>& arr) {
    if (arr.size() <= 1) return arr;
    
    std::vector<Node<T>*> nodes;
    std::vector<Node<T>*> winners;
    std::vector<Node<T>*> losers;
    bool hasOdd = false;
    Node<T>* oddElement = nullptr;
    
    for (size_t i = 0; i < arr.size(); ++i) {
        nodes.push_back(new Node<T>(&arr[i]));
    }
    
    for (size_t i = 0; i < nodes.size(); i += 2) {
        if (i + 1 < nodes.size()) {
            Node<T>* first = nodes[i];
            Node<T>* second = nodes[i + 1];
            ++g_vecComparisonCount;
            if (first->get() > second->get()) {
                winners.push_back(first);
                first->setPair(second);
            } else {
                winners.push_back(second);
                second->setPair(first);
            }
        } else {
            hasOdd = true;
            oddElement = nodes[i];
        }
    }
    
    if (winners.size() > 1) {
        std::vector<T> tempVec;
        for (size_t i = 0; i < winners.size(); ++i) {
            tempVec.push_back(winners[i]->get());
        }
        
        std::vector<T> sortedVec = fordJohnsonVec(tempVec);
        
        std::vector<Node<T>*> newWinners;
        for (size_t i = 0; i < sortedVec.size(); ++i) {
            for (size_t j = 0; j < winners.size(); ++j) {
                if (winners[j]->get() == sortedVec[i]) {
                    newWinners.push_back(winners[j]);
                    break;
                }
            }
        }
        winners = newWinners;
    }
    
    std::vector<T> result;
    
    if (!winners.empty() && winners[0]->hasPair()) {
        result.push_back(winners[0]->getPair()->get());
    }
    
    for (size_t i = 0; i < winners.size(); ++i) {
        result.push_back(winners[i]->get());
    }
    
    if (winners.size() > 1) {
        std::vector<Node<T>*> remainingLosers;
        for (size_t i = 1; i < winners.size(); ++i) {
            if (winners[i]->hasPair()) {
                remainingLosers.push_back(winners[i]->getPair());
            }
        }
        
        if (hasOdd) {
            remainingLosers.push_back(oddElement);
        }
        
        size_t insertedCount = 0;
        
        for (size_t k = 1; insertedCount < remainingLosers.size(); ++k) {
            size_t jacobsthalK = nextJacobsthal(k);
            
            size_t start = insertedCount;
            size_t end = std::min(jacobsthalK - 1, remainingLosers.size() - 1);
            
            for (size_t idx = end; idx != SIZE_MAX && idx >= start; --idx) {
                if (idx < remainingLosers.size()) {
                    T val = remainingLosers[idx]->get();
                    typename std::vector<T>::iterator searchEnd = result.end();
                    Node<T>* pairedWinner = nullptr;
                    for (size_t w = 0; w < winners.size(); ++w) {
                        if (winners[w]->getPair() == remainingLosers[idx]) {
                            pairedWinner = winners[w];
                            break;
                        }
                    }
                    
                    if (pairedWinner) {
                        for (typename std::vector<T>::iterator it = result.begin(); it != result.end(); ++it) {
                            if (*it == pairedWinner->get()) {
                                searchEnd = it;
                                break;
                            }
                        }
                    }
                    
                    typename std::vector<T>::iterator pos = optimizedLowerBound(
                        result.begin(), searchEnd, val, g_vecComparisonCount);
                    result.insert(pos, val);
                }
            }
            insertedCount = end + 1;
        }
    } else if (hasOdd) {
        T val = oddElement->get();
        typename std::vector<T>::iterator pos = optimizedLowerBound(
            result.begin(), result.end(), val, g_vecComparisonCount);
        result.insert(pos, val);
    }
    
    for (size_t i = 0; i < nodes.size(); ++i) {
        delete nodes[i];
    }
    
    return result;
}

template <typename T>
std::list<T> PmergeMe::fordJohnsonList(std::list<T> &arr) {
    if (arr.size() <= 1) 
        return arr;  
        
    std::vector<Node<T>*> nodes;
    std::vector<Node<T>*> winners;
    std::vector<Node<T>*> losers;
    bool hasOdd = false;
    Node<T>* oddElement = nullptr;
    
    for (typename std::list<T>::iterator it = arr.begin(); it != arr.end(); ++it) {
        nodes.push_back(new Node<T>(&(*it)));
    }
    
    for (size_t i = 0; i < nodes.size(); i += 2) {
        if (i + 1 < nodes.size()) {
            Node<T>* first = nodes[i];
            Node<T>* second = nodes[i + 1];
            ++g_listComparisonCount;
            if (first->get() > second->get()) {
                winners.push_back(first);
                first->setPair(second);
            } else {
                winners.push_back(second);
                second->setPair(first);
            }
        } else {
            hasOdd = true;
            oddElement = nodes[i];
        }
    }
    
    if (winners.size() > 1) {
        std::list<T> tempList;
        for (size_t i = 0; i < winners.size(); ++i) {
            tempList.push_back(winners[i]->get());
        }
        
        std::list<T> sortedList = fordJohnsonList(tempList);
        
        std::vector<Node<T>*> newWinners;
        for (typename std::list<T>::iterator it = sortedList.begin(); it != sortedList.end(); ++it) {
            for (size_t j = 0; j < winners.size(); ++j) {
                if (winners[j]->get() == *it) {
                    newWinners.push_back(winners[j]);
                    break;
                }
            }
        }
        winners = newWinners;
    }
    
    std::list<T> result;
    
    if (!winners.empty() && winners[0]->hasPair()) {
        result.push_back(winners[0]->getPair()->get());
    }
    
    for (size_t i = 0; i < winners.size(); ++i) {
        result.push_back(winners[i]->get());
    }
    
    if (winners.size() > 1) {
        std::vector<Node<T>*> remainingLosers;
        for (size_t i = 1; i < winners.size(); ++i) {
            if (winners[i]->hasPair()) {
                remainingLosers.push_back(winners[i]->getPair());
            }
        }
        
        if (hasOdd) {
            remainingLosers.push_back(oddElement);
        }
        
        std::vector<T> tmpResult(result.begin(), result.end());
        size_t insertedCount = 0;
        
        for (size_t k = 1; insertedCount < remainingLosers.size(); ++k) {
            size_t jacobsthalK = nextJacobsthal(k);
            
            size_t start = insertedCount;
            size_t end = std::min(jacobsthalK - 1, remainingLosers.size() - 1);
            
            for (size_t idx = end; idx != SIZE_MAX && idx >= start; --idx) {
                if (idx < remainingLosers.size()) {
                    T val = remainingLosers[idx]->get();
                    
                    typename std::vector<T>::iterator searchEnd = tmpResult.end();
                    Node<T>* pairedWinner = nullptr;
                    for (size_t w = 0; w < winners.size(); ++w) {
                        if (winners[w]->getPair() == remainingLosers[idx]) {
                            pairedWinner = winners[w];
                            break;
                        }
                    }
                    
                    if (pairedWinner) {
                        for (typename std::vector<T>::iterator it = tmpResult.begin(); it != tmpResult.end(); ++it) {
                            if (*it == pairedWinner->get()) {
                                searchEnd = it;
                                break;
                            }
                        }
                    }
                    
                    typename std::vector<T>::iterator pos = optimizedLowerBound(
                        tmpResult.begin(), searchEnd, val, g_listComparisonCount);
                    tmpResult.insert(pos, val);
                }
            }
            
            insertedCount = end + 1;
        }
        
        result.clear();
        result.insert(result.begin(), tmpResult.begin(), tmpResult.end());
    } else if (hasOdd) {
        std::vector<T> tmpResult(result.begin(), result.end());
        T val = oddElement->get();
        typename std::vector<T>::iterator pos = optimizedLowerBound(
            tmpResult.begin(), tmpResult.end(), val, g_listComparisonCount);
        tmpResult.insert(pos, val);
        result.clear();
        result.insert(result.begin(), tmpResult.begin(), tmpResult.end());
    }
    for (size_t i = 0; i < nodes.size(); ++i) {
        delete nodes[i];
    }
    return result;
}

template std::vector<int> PmergeMe::fordJohnsonVec(std::vector<int>& arr);
template std::list<int> PmergeMe::fordJohnsonList(std::list<int>& lst);

size_t PmergeMe::nextJacobsthal(size_t n) {
    if (n == 0) return 0;
    if (n == 1) return 2;
    
    size_t a = 2;
    size_t b = 0;
    size_t result = 0;
    
    for (size_t i = 2; i <= n; ++i) {
        result = b + 2 * a;
        a = b;
        b = result;
    }
    
    return result;
}