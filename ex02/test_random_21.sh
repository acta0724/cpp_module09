#!/bin/bash

# 21個のランダムな要素を生成するスクリプト
echo "21個のランダムな整数でテストを実行します"

# ランダムな整数を生成
RANDOM_NUMBERS=""
for i in {1..21}
do
  # 1から1000までのランダムな整数を生成
  NUM=$((RANDOM % 10000 + 1))
  RANDOM_NUMBERS="$RANDOM_NUMBERS $NUM"
done

echo "生成された数値: $RANDOM_NUMBERS"
echo "ソートを実行中..."

# PmergeMeを実行
./PmergeMe $RANDOM_NUMBERS
