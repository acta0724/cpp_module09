#include "PmergeMe.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <limits>
#include <cerrno>
#include <iomanip>
#include <sys/time.h>
#include "PmergeMe.hpp"

// 時間計測用関数
unsigned long getTimeInMicroseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

bool isPositiveInteger(const std::string& str) {
    if (str.empty()) 
		return false;
    
    for (size_t i = 0; i < str.length(); ++i) {
        if (!std::isdigit(str[i])) 
			return false;
    }
    
    return true;
}

bool stringToInt(const std::string& str, int& value) {
    std::istringstream iss(str);
    iss >> value;
    return !iss.fail() && iss.eof() && value >= 0;
}

int main(int argc, char **argv) {
    try {
        // 引数のチェック
        if (argc < 2) {
            std::cerr << "Error: 引数が必要です。整数のリストを指定してください。" << std::endl;
            return EXIT_FAILURE;
        }
        
        // 入力の解析
        std::vector<int> vec;
        std::vector<int> originalVec;
        std::list<int> lst;
        
        // 全ての引数を処理
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            std::istringstream iss(arg);
            std::string token;
            
            // 各引数をトークンに分割
            while (iss >> token) {
                int value;
                if (!isPositiveInteger(token) || !stringToInt(token, value)) {
                    std::cerr << "Error: 無効な引数 '" << token << "'。正の整数のみ使用可能です。" << std::endl;
                    return EXIT_FAILURE;
                }
                vec.push_back(value);
                originalVec.push_back(value);
                lst.push_back(value);
            }
        }
        
        if (vec.empty()) {
            std::cerr << "Error: 有効な整数が指定されていません。" << std::endl;
            return EXIT_FAILURE;
        }
        
        // 元のデータを表示
        std::cout << "Before vector: ";
        for (size_t i = 0; i < originalVec.size() && i < 10; ++i) {
            std::cout << originalVec[i] << " ";
        }
        if (originalVec.size() > 10) {
            std::cout << "[...]";
        }
        std::cout << std::endl;
        
        // 比較回数カウンタをリセット
        PmergeMe::resetComparisonCounts();
        
        // vectorでのソート
        unsigned long startVec = getTimeInMicroseconds();
        std::vector<int> sortedVec = PmergeMe::fordJohnsonVec(vec);
        unsigned long endVec = getTimeInMicroseconds();
        unsigned long vecTime = endVec - startVec;
        size_t vecComparisons = PmergeMe::getVecComparisonCount();
        
        // 比較回数カウンタをリセット
        PmergeMe::resetComparisonCounts();
        
        // listでのソート
        unsigned long startList = getTimeInMicroseconds();
        std::list<int> sortedList = PmergeMe::fordJohnsonList(lst);
        unsigned long endList = getTimeInMicroseconds();
        unsigned long listTime = endList - startList;
        size_t listComparisons = PmergeMe::getListComparisonCount();
        
        // 結果を表示
        std::cout << "After vector: ";
        for (size_t i = 0; i < sortedVec.size(); ++i) {
            std::cout << sortedVec[i] << " ";
        }
        if (sortedVec.size() > 10) {
            std::cout << "[...]";
        }
        std::cout << std::endl;

		std::cout << "corrent sorted: ";
		for (size_t i = 1; i < originalVec.size() + 1; ++i) {
			std::cout << i << " ";
		}
		std::cout << std::endl;

		std::cout << "After list:     ";
        std::list<int>::iterator it = sortedList.begin();
        for (size_t i = 0; i < sortedList.size(); ++i) {
            if (it != sortedList.end()) {
                std::cout << *it << " ";
                ++it;
            }
        }
        // if (sortedList.size() > 10) {
        //     std::cout << "[...]";
        // }
        std::cout << std::endl;
        
        // ソートが正しく行われたか確認
        bool isSortedVec = std::is_sorted(sortedVec.begin(), sortedVec.end());
        if (!isSortedVec) {
            std::cerr << "Error: vectorのソートが正しく行われていません。" << std::endl;
            return EXIT_FAILURE;
        }
        
        // パフォーマンス情報を表示
        std::cout << "Time to process a range of " 
                  << std::setw(5) 
                  << std::setfill(' ') << originalVec.size()
                  << " elements with std::vector : "
                  << vecTime << " us" 
                  << "(" << vecComparisons << " comparisons)" << std::endl;
        
        std::cout << "Time to process a range of " 
                  << std::setw(5) 
                  << std::setfill(' ') << originalVec.size()
                  << " elements with std::list : "
                  << listTime << " us"
                  << "(" << listComparisons << " comparisons)" << std::endl;
        
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Error: 不明なエラーが発生しました。" << std::endl;
        return EXIT_FAILURE;
    }
}
