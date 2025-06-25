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

template <typename T>
std::list<T> PmergeMe::fordJohnsonList(std::list<T> &arr) {
    if (arr.size() <= 1) 
		return arr;  
    std::list<Node<T>*> nodes;
    std::list<Node<T>*> winners;
    std::list<Node<T>*> losers;
	bool odd = false;
    for (typename std::list<T>::iterator it = arr.begin(); it != arr.end(); ++it) {
        nodes.push_back(new Node<T>(&(*it)));
    }
    typename std::list<Node<T>*>::iterator it = nodes.begin();
    while (it != nodes.end()) {
        typename std::list<Node<T>*>::iterator next = it;
        ++next;
        if (next != nodes.end()) {
            Node<T>* first = *it;
            Node<T>* second = *next;
            ++g_listComparisonCount; 
            if (first->get() > second->get()) {
                winners.push_back(first);
                losers.push_back(second);
                first->setPair(second);
            } else {
                winners.push_back(second);
                losers.push_back(first);
                second->setPair(first);
            }
            it = ++next;
        } else {
			odd = true;
            losers.push_back(*it);
            ++it;
        }
    }
    if (winners.size() > 1) {
        std::list<T> tempList;
        for (typename std::list<Node<T>*>::iterator it = winners.begin(); it != winners.end(); ++it) {
            tempList.push_back((*it)->get());
        }
        std::list<T> sortedList = fordJohnsonList(tempList);
        winners.clear();
        for (typename std::list<T>::iterator it = sortedList.begin(); it != sortedList.end(); ++it) {
            for (typename std::list<Node<T>*>::iterator nodeIt = nodes.begin(); nodeIt != nodes.end(); ++nodeIt) {
                if ((*nodeIt)->get() == *it) {
                    winners.push_back(*nodeIt);
                    break;
                }
            }
        }
    }
    std::list<T> result;
    for (typename std::list<Node<T>*>::iterator it = winners.begin(); it != winners.end(); ++it) {
        result.push_back((*it)->get());
    }
    if (!losers.empty()) {
		std::vector<Node<T>*> losersVec;
		for (typename std::list<Node<T>*>::iterator it = winners.begin(); it != winners.end(); ++it) {
			if ((*it)->hasPair()) {
				losersVec.push_back((*it)->getPair());
			}
		}
		if (odd) {
			losersVec.push_back(losers.back());
		}
        std::vector<T> tmpResult(result.begin(), result.end());
        size_t processedCount = 0;
        for (size_t n = 0; ; ++n) {
			size_t jn = 2 * nextJacobsthal(n);
			size_t nextIndex = processedCount + jn;
			if (nextIndex >= losersVec.size()) {
				nextIndex = losersVec.size() - 1;
			}
            std::vector<Node<T>*> tmpLosers;
            for (size_t i = processedCount; i <= nextIndex; ++i) {
                tmpLosers.push_back(losersVec[i]);
            }
            for (int i = static_cast<int>(tmpLosers.size()) - 1; i >= 0; --i) {
                Node<T>* loser = tmpLosers[i];
                T val = loser->get();
                typename std::vector<T>::iterator end = tmpResult.end() -1;
                // Custom binary search implementation instead of std::lower_bound
                typename std::vector<T>::iterator start = tmpResult.begin();
                typename std::vector<T>::iterator last = end;
                typename std::vector<T>::iterator pos;
                ListCompare<T> comp;
                
                while (start < last) {
                    typename std::vector<T>::iterator mid = start + (last - start) / 2;
                    if (comp(*mid, val)) {
                        // If mid element is less than val, search in right half
                        start = mid + 1;
                    } else {
                        // If mid element is greater or equal to val, search in left half
                        last = mid;
                    }
                }
                pos = start;
                tmpResult.insert(pos, val);
            }
			std::cout << "g_listComparisonCount: " << g_listComparisonCount << std::endl;
            processedCount = nextIndex + 1;
            if (processedCount >= losersVec.size() - 1) {
                break;
            }
        }       
        result.clear();
        result.insert(result.begin(), tmpResult.begin(), tmpResult.end());
    }
    for (typename std::list<Node<T>*>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
        delete *it;
    }
    return result;
}

// テンプレート関数の実装 - Node型を使用したfordJohnsonVec関数
template <typename T>
std::vector<T> PmergeMe::fordJohnsonVec(std::vector<T>& arr) {
    // Base case: if array has 0 or 1 element, it's already sorted
    if (arr.size() <= 1) return arr;
    
    // Node型のベクトル作成
    std::vector<Node<T>*> nodes;
    std::vector<Node<T>*> winners;
    std::vector<Node<T>*> losers;
    
    // Nodeを作成
    for (size_t i = 0; i < arr.size(); ++i) {
        nodes.push_back(new Node<T>(&arr[i]));
    }
    
    // ① ペアリング処理
    
    for (size_t i = 0; i < nodes.size(); i += 2) {
        if (i + 1 < nodes.size()) {
            Node<T>* first = nodes[i];
            Node<T>* second = nodes[i + 1];
            ++g_vecComparisonCount;
            if (first->get() > second->get()) {
                winners.push_back(first);
                losers.push_back(second);
                first->setPair(second);
            } else {
                winners.push_back(second);
                losers.push_back(first);
                second->setPair(first);
            }
        } else {
            losers.push_back(nodes[i]);
        }
    }
    
    // ② winnersを再帰的にソート
    if (winners.size() > 1) {
        // 一時的にwinnersの値をベクトルに変換
        std::vector<T> tempVec;
        for (size_t i = 0; i < winners.size(); ++i) {
            tempVec.push_back(winners[i]->get());
        }
        
        // 再帰的にソート
        std::vector<T> sortedVec = fordJohnsonVec(tempVec);
        
        // ソート結果でwinnersを更新
        winners.clear();
        for (size_t i = 0; i < sortedVec.size(); ++i) {
            // 対応するNodeを探す
            for (size_t j = 0; j < nodes.size(); ++j) {
                if (nodes[j]->get() == sortedVec[i]) {
                    winners.push_back(nodes[j]);
                    break;
                }
            }
        }
    }
    
    // ③ 結果ベクトルの作成
    std::vector<T> result;
    
    // ソートされたwinnersを結果に追加
    for (size_t i = 0; i < winners.size(); ++i) {
        result.push_back(winners[i]->get());
    }
    
    // ④ Jacobsthal数列を使って残りのlosersを挿入
    if (!losers.empty()) {
        // 最初の敗者（インデックス0）は特別に処理
        if (!losers.empty()) {
            Node<T>* firstLoser = losers[0];
            T val = firstLoser->get();
            
            // 手動で適切な挿入位置を探す
            typename std::vector<T>::iterator insertPos = result.begin();
            while (insertPos != result.end() && *insertPos < val) {
                ++insertPos;
                ++g_vecComparisonCount; // 比較ごとにカウントを増やす
            }
            
            // 見つけた位置に挿入
            result.insert(insertPos, val);
			typename std::vector<Node<T>*>::iterator it = std::find(losers.begin(), losers.end(), firstLoser);
			if (it != losers.end()) {
				losers.erase(it);
			}
        }
        
        // Jacobsthal数列に基づいて敗者を処理
        size_t processedCount = 1; // 最初の敗者は既に処理済み
        
        for (size_t n = 1; ; ++n) {
            // Jacobsthal数を計算
            size_t jn = nextJacobsthal(n);
            // 次の処理する敗者のインデックスを計算
            size_t nextIndex = processedCount + jn;
            if (nextIndex >= losers.size()) {
                nextIndex = losers.size() - 1;
            }
            
            // 一時的な敗者リスト
            std::vector<Node<T>*> tmpLosers;
            
            // processedCount から nextIndex までの敗者を追加
            for (size_t i = processedCount; i <= nextIndex && i < losers.size(); ++i) {
                tmpLosers.push_back(losers[i]);
            }
            
            // 一時リストの敗者を逆順に処理（大きい方から）
            for (int i = static_cast<int>(tmpLosers.size() - 1); i >= 0; --i) {
                Node<T>* loser = tmpLosers[i];
                T val = loser->get();
                
                // std::lower_boundを使用して挿入位置を探索
                typename std::vector<T>::iterator pos = std::lower_bound(
                    result.begin(), result.end(), val, VecCompare<T>());
                
                // 見つけた位置に挿入
                result.insert(pos, val);
            }
            
            // 処理済み敗者数を更新
            processedCount = nextIndex + 1;
            
            // すべての敗者を処理したら終了
            if (processedCount >= losers.size()) {
                break;
            }
        }
    }
    
    // ⑤ メモリ解放
    for (size_t i = 0; i < nodes.size(); ++i) {
        delete nodes[i];
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
