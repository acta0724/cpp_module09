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
    bool operator()(const T& a, const T& b) const {
        ++g_vecComparisonCount;
        return a < b;
    }
};

template <typename T>
class ListCompare {
public:
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
    
    void setPair(Node<T>* p) {
        pair = p;
    }
    
    Node<T>* popPair() {
        Node<T>* p = pair;
        pair = NULL;
        return p;
    }
    
    bool hasPair() const {
        return pair != NULL;
    }
	
    T& get() const {
        return *value;
    }
    
    T* getValuePtr() const {
        return value;
    }
    
    Node<T>* getPair() const {
        return pair;
    }
    
    bool operator<(const Node<T>& other) const {
        return *value < *other.value;
    }
};

class PmergeMe {
private:
	PmergeMe();
	PmergeMe(const PmergeMe &src);
	PmergeMe &operator=(const PmergeMe &src);
	~PmergeMe();

	static size_t vecComparisonCount;
	static size_t listComparisonCount;

	static size_t nextJacobsthal(size_t n);

	template <typename T>
	static void createPairsVec(std::vector<Node<T>*>& nodes, std::vector<Node<T>*>& winners, Node<T>*& oddElement, bool& hasOdd);

	template <typename T>
	static void createPairsList(std::list<Node<T>*>& nodes, std::list<Node<T>*>& winners, Node<T>*& oddElement, bool& hasOdd);

public:
	static void resetComparisonCounts();
	static size_t getVecComparisonCount();
	static size_t getListComparisonCount();

	template <typename T>
	static std::vector<T> fordJohnsonVec(std::vector<T>& arr);
	
	template <typename T>
	static std::list<T> fordJohnsonList(std::list<T> &lst);
};

#endif
