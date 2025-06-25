#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <list>
#include <iostream>

// 比較回数を外部変数として定義
extern size_t g_vecComparisonCount;
extern size_t g_listComparisonCount;

// ベクトル用比較関数
template <typename T>
class VecCompare {
public:
    bool operator()(const T& a, const T& b) const {
        ++g_vecComparisonCount;
        return a < b;
    }
};

// リスト用比較関数
template <typename T>
class ListCompare {
public:
    bool operator()(const T& a, const T& b) const {
        ++g_listComparisonCount;
        return a < b;
    }
};

// Node型の定義 - ペア情報を保持するためのクラス
template<typename T>
class Node {
private:
    T* value;         // 値へのポインタ
    Node<T>* pair;    // 敵ペアへのポインタ

public:
    // コンストラクタ
    Node() : value(NULL), pair(NULL) {}
    Node(T* v) : value(v), pair(NULL) {}
    
    // ペアを設定
    void setPair(Node<T>* p) {
        pair = p;
    }
    
    // ペアを取得して切断
    Node<T>* popPair() {
        Node<T>* p = pair;
        pair = NULL;
        return p;
    }
    
    // ペアがあるか確認
    bool hasPair() const {
        return pair != NULL;
    }
    
    // 値を取得
    T& get() const {
        return *value;
    }
    
    // 値のポインタを取得
    T* getValuePtr() const {
        return value;
    }
    
    // ペアを取得
    Node<T>* getPair() const {
        return pair;
    }
    
    // 比較演算子
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

	// 比較回数カウンタ
	static size_t vecComparisonCount;
	static size_t listComparisonCount;

	static size_t nextJacobsthal(size_t n);

public:
	// 比較回数カウンタをリセット
	static void resetComparisonCounts();
	
	// ベクトルの比較回数を取得
	static size_t getVecComparisonCount();
	
	// リストの比較回数を取得
	static size_t getListComparisonCount();

	// Ford-Johnson（マージ挿入ソート）アルゴリズムを使用してベクトルをソート
	template <typename T>
	static std::vector<T> fordJohnsonVec(std::vector<T>& arr);
	
	// リストをソート
	template <typename T>
	static std::list<T> fordJohnsonList(std::list<T> &lst);
};

#endif
