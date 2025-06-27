#!/bin/bash

# 重複しないランダムな要素を生成するスクリプト
echo "重複しないランダムな整数でテストを実行します"

# 生成する数値の数
COUNT=21

# ランダムな整数を生成（重複チェック付き）
RANDOM_NUMBERS=""
GENERATED_COUNT=0

while [ $GENERATED_COUNT -lt $COUNT ]
do
  # 1から100000000までのランダムな整数を生成
  NUM=$((RANDOM % 100000000 + 1))
  
  # 重複チェック - echoとgrepを使用
  if ! echo "$RANDOM_NUMBERS" | grep -wq "$NUM"; then
    RANDOM_NUMBERS="$RANDOM_NUMBERS $NUM"
    GENERATED_COUNT=$((GENERATED_COUNT + 1))
  fi
done

echo "生成された数値: $RANDOM_NUMBERS"
echo "ソートを実行中..."

# PmergeMeを実行
./PmergeMe $RANDOM_NUMBERS
