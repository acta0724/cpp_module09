#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <list>
#include <iostream>

extern size_t g_vecComparisonCount;
extern size_t g_listComparisonCount;

template <typename T>
class VecCompare {
public:
	VecCompare() {}
	VecCompare(const VecCompare &) {}
	VecCompare &operator=(const VecCompare &) { return *this; }
	~VecCompare() {}
    bool operator()(const T& a, const T& b) const {
        ++g_vecComparisonCount;
        return a < b;
    }
};

template <typename T>
class ListCompare {
public:
	ListCompare() {}
	ListCompare(const ListCompare &) {}
	ListCompare &operator=(const ListCompare &) { return *this; }
	~ListCompare() {}
    bool operator()(const T& a, const T& b) const {
        ++g_listComparisonCount;
        return a < b;
    }
};

template<typename T>
class Node {
private:
    T* value;
    Node<T>* pair;

public:
    Node() : value(NULL), pair(NULL) {}
    Node(T* v) : value(v), pair(NULL) {}
	Node(const Node &src) : value(src.value), pair(src.pair) {}
	Node &operator=(const Node &src) {
		if (this != &src) {
			value = src.value;
			pair = src.pair;
		}
		return *this;
	}
	~Node() {}
    void setPair(Node<T>* p) {
        pair = p;
    }
    bool hasPair() const {
        return pair != NULL;
    }
    T& get() const {
        return *value;
    }
    Node<T>* getPair() const {
        return pair;
    }
};

class PmergeMe {
private:
	PmergeMe() {};
	PmergeMe(const PmergeMe &src) {
		(void)src;
	}
	PmergeMe &operator=(const PmergeMe &src) {
		(void)src;
		return *this;
	}
	~PmergeMe() {};
	static size_t nextJacobsthal(size_t n);
	template <typename T>
	static void createPairsVec(std::vector<Node<T>*>& nodes, std::vector<Node<T>*>& winners, Node<T>*& oddElement, bool& hasOdd);
	template <typename T>
	static void createPairsList(std::list<Node<T>*>& nodes, std::list<Node<T>*>& winners, Node<T>*& oddElement, bool& hasOdd);
	template <typename T>
	static Node<T>* findPairedWinner(const std::list<Node<T>*>& winners, Node<T>* loser);

	template <typename T>
	static typename std::list<T>::iterator findSearchEnd(std::list<T>& result, Node<T>* pairedWinner);

	template <typename T>
	static std::list<Node<T>*> reorderWinners(std::list<Node<T>*>& winners, const std::list<T>& sortedList);

	template <typename T>
	static std::vector<Node<T>*> reorderWinners(std::vector<Node<T>*>& winners, const std::vector<T>& sortedVec);

	template <typename T>
	static Node<T>* findPairedWinner(const std::vector<Node<T>*>& winners, Node<T>* loser);

	template <typename T>
	static typename std::vector<T>::iterator findSearchEnd(std::vector<T>& result, Node<T>* pairedWinner);

public:
	static size_t getVecComparisonCount() { return g_vecComparisonCount; }
	static size_t getListComparisonCount() { return g_listComparisonCount; }
	template <typename T>
	static std::vector<T> fordJohnsonVec(std::vector<T>& arr);
	template <typename T>
	static std::list<T> fordJohnsonList(std::list<T> &lst);
};

#endif
