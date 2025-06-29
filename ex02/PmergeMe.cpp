#include "PmergeMe.hpp"
#include <algorithm>
#include <utility>

#define SIZE_MAX static_cast<size_t>(-1)

size_t g_vecComparisonCount = 0;
size_t g_listComparisonCount = 0;

template <typename T>
std::vector<T> PmergeMe::fordJohnsonVec(std::vector<T>& arr) {
    if (arr.size() <= 1) {
		return arr;
    }
    std::vector<Node<T>*> nodes;
    std::vector<Node<T>*> winners;
	std::vector<T> sortedVec;
	std::vector<T> tempVec;
	std::vector<T> result;
	std::vector<Node<T>*> newLosers;
    bool hasOdd = false;
    Node<T>* oddElement = NULL;
	Node<T>* pairedWinner = NULL;
	size_t insertedCount = 0;
	size_t jacobsthalK = 0;
	size_t start = 0;
	size_t end = 0;
	T val;
	typename std::vector<Node<T>*>::iterator loserIt;
	typename std::vector<Node<T>*>::iterator w_it;
	typename std::vector<T>::iterator searchEnd;
	typename std::vector<T>::iterator insertPos;
	VecCompare<T> comparator;
    for (size_t i = 0; i < arr.size(); ++i) {
        nodes.push_back(new Node<T>(&arr[i]));
    }
    createPairsVec<T>(nodes, winners, oddElement, hasOdd);
	for (size_t i = 0; i < winners.size(); ++i) {
		tempVec.push_back(winners[i]->get());
	}
	sortedVec = fordJohnsonVec(tempVec);
	winners = reorderWinners(winners, sortedVec);
    if (!winners.empty() && winners[0]->hasPair()) {
        result.push_back(winners[0]->getPair()->get());
    }
    for (size_t i = 0; i < winners.size(); ++i) {
        result.push_back(winners[i]->get());
    }
    for (size_t i = 1; i < winners.size(); ++i) {
        if (winners[i]->hasPair()) {
            newLosers.push_back(winners[i]->getPair());
        }
    }
    if (hasOdd) {
        newLosers.push_back(oddElement);
    }
    for (size_t k = 1; insertedCount < newLosers.size(); ++k) {
        jacobsthalK = nextJacobsthal(k);
        start = insertedCount;
        end = std::min(jacobsthalK - 1, newLosers.size() - 1);
        for (size_t idx = end; idx != SIZE_MAX && idx >= start; --idx) {
			val = newLosers[idx]->get();
			pairedWinner = findPairedWinner(winners, newLosers[idx]);
			searchEnd = findSearchEnd(result, pairedWinner);
			insertPos = std::lower_bound(result.begin(), searchEnd, val, comparator);
			result.insert(insertPos, val);
            insertedCount = end + 1;
		}
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
    std::list<Node<T>*> nodes;
    std::list<Node<T>*> winners;
	std::list<T> tempList;
	std::list<T> result;
	std::list<T> sortedList;
	std::list<Node<T>*> newLosers;
    bool hasOdd = false;
    Node<T>* oddElement = NULL;
	Node<T>* pairedWinner = NULL;
	size_t insertedCount = 0;
	size_t jacobsthalK = 0;
	size_t start = 0;
	size_t end = 0;
	T val;
	typename std::list<Node<T>*>::iterator loserIt;
	typename std::list<Node<T>*>::iterator w_it;
	typename std::list<T>::iterator searchEnd;
	typename std::list<T>::iterator insertPos;
	ListCompare<T> comparator;

    for (typename std::list<T>::iterator it = arr.begin(); it != arr.end(); ++it) {
        nodes.push_back(new Node<T>(&(*it)));
    }
    createPairsList<T>(nodes, winners, oddElement, hasOdd);
    for (typename std::list<Node<T>*>::iterator it = winners.begin(); it != winners.end(); ++it) {
        tempList.push_back((*it)->get());
    }
    sortedList = fordJohnsonList(tempList);
    winners = reorderWinners(winners, sortedList);
    if (!winners.empty() && winners.front()->hasPair()) {
        result.push_back(winners.front()->getPair()->get());
    }
    for (typename std::list<Node<T>*>::iterator it = winners.begin(); it != winners.end(); ++it) {
        result.push_back((*it)->get());
    }
    w_it = winners.begin();
    if (w_it != winners.end()) {
        ++w_it;
    }
    for (; w_it != winners.end(); ++w_it) {
        if ((*w_it)->hasPair()) {
            newLosers.push_back((*w_it)->getPair());
        }
    }
    if (hasOdd) {
        newLosers.push_back(oddElement);
    }
    for (size_t k = 1; insertedCount < newLosers.size(); ++k) {
        jacobsthalK = nextJacobsthal(k);
        start = insertedCount;
        end = std::min(jacobsthalK - 1, newLosers.size() - 1);
        for (size_t idx = end; idx != SIZE_MAX && idx >= start; --idx) {
            loserIt = newLosers.begin();
            std::advance(loserIt, idx);
            val = (*loserIt)->get();
			pairedWinner = findPairedWinner(winners, *loserIt);
			searchEnd = findSearchEnd(result, pairedWinner);
			insertPos = std::lower_bound(result.begin(), searchEnd, val, comparator);
			result.insert(insertPos, val);
		}
		insertedCount = end + 1;
    }
    for (typename std::list<Node<T>*>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
        delete *it;
    }
    return result;
}

template std::vector<int> PmergeMe::fordJohnsonVec(std::vector<int>& arr);
template std::list<int> PmergeMe::fordJohnsonList(std::list<int>& lst);

template <typename T>
void PmergeMe::createPairsVec(std::vector<Node<T>*>& nodes, std::vector<Node<T>*>& winners, Node<T>*& oddElement, bool& hasOdd) {
	Node<T>* first = NULL;
	Node<T>* second = NULL;
    for (size_t i = 0; i < nodes.size(); i += 2) {
        if (i + 1 < nodes.size()) {
            first = nodes[i];
            second = nodes[i + 1];
            ++g_vecComparisonCount;
            if (first->get() > second->get()) {
                winners.push_back(first);
                first->setPair(second);
				second->setPair(first);
            } else {
                winners.push_back(second);
                second->setPair(first);
				first->setPair(second);
            }
        } else {
            hasOdd = true;
            oddElement = nodes[i];
        }
    }
}

template <typename T>
void PmergeMe::createPairsList(std::list<Node<T>*>& nodes, std::list<Node<T>*>& winners, Node<T>*& oddElement, bool& hasOdd) {
	Node<T>* first = NULL;
	Node<T>* second = NULL;
    for (typename std::list<Node<T>*>::iterator it = nodes.begin(); it != nodes.end(); ) {
        first = *it;
        ++it;
        if (it != nodes.end()) {
            second = *it;
            ++it;
            ++g_listComparisonCount;
            if (first->get() > second->get()) {
                winners.push_back(first);
                first->setPair(second);
				second->setPair(first);
            } else {
                winners.push_back(second);
                second->setPair(first);
				first->setPair(second);
            }
        } else {
            hasOdd = true;
            oddElement = first;
        }
    }
}

size_t PmergeMe::nextJacobsthal(size_t n) {
    if (n == 0) 
		return 0;
    if (n == 1) 
		return 2;
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

template <typename T>
Node<T>* PmergeMe::findPairedWinner(const std::list<Node<T>*>& winners, Node<T>* loser) {
    for (typename std::list<Node<T>*>::const_iterator it = winners.begin(); it != winners.end(); ++it) {
        if ((*it)->getPair() == loser) {
            return *it;
        }
    }
    return NULL;
}

template <typename T>
typename std::list<T>::iterator PmergeMe::findSearchEnd(std::list<T>& result, Node<T>* pairedWinner) {
    if (!pairedWinner) {
        return result.end();
    }
    for (typename std::list<T>::iterator it = result.begin(); it != result.end(); ++it) {
        if (*it == pairedWinner->get()) {
            return it;
        }
    }
    return result.end();
}

template <typename T>
std::list<Node<T>*> PmergeMe::reorderWinners(std::list<Node<T>*>& winners, const std::list<T>& sortedList) {
    std::list<Node<T>*> newWinners;
    
    for (typename std::list<T>::const_iterator it = sortedList.begin(); it != sortedList.end(); ++it) {
        for (typename std::list<Node<T>*>::iterator w_it = winners.begin(); w_it != winners.end(); ++w_it) {
            if ((*w_it)->get() == *it) {
                newWinners.push_back(*w_it);
                winners.erase(w_it);
                break;
            }
        }
    }    
    return newWinners;
}

template <typename T>
std::vector<Node<T>*> PmergeMe::reorderWinners(std::vector<Node<T>*>& winners, const std::vector<T>& sortedVec) {
    std::vector<Node<T>*> newWinners;
    
    for (typename std::vector<T>::const_iterator it = sortedVec.begin(); it != sortedVec.end(); ++it) {
        for (typename std::vector<Node<T>*>::iterator w_it = winners.begin(); w_it != winners.end(); ++w_it) {
            if ((*w_it)->get() == *it) {
                newWinners.push_back(*w_it);
                break;
            }
        }
    }
    return newWinners;
}

template <typename T>
Node<T>* PmergeMe::findPairedWinner(const std::vector<Node<T>*>& winners, Node<T>* loser) {
    for (typename std::vector<Node<T>*>::const_iterator it = winners.begin(); it != winners.end(); ++it) {
        if ((*it)->getPair() == loser) {
            return *it;
        }
    }
    return NULL;
}

template <typename T>
typename std::vector<T>::iterator PmergeMe::findSearchEnd(std::vector<T>& result, Node<T>* pairedWinner) {
    if (!pairedWinner) {
        return result.end();
    }
    for (typename std::vector<T>::iterator it = result.begin(); it != result.end(); ++it) {
        if (*it == pairedWinner->get()) {
            return it;
        }
    }
    return result.end();
}
