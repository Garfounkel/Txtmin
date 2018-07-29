#pragma once

#include <utility>

template <typename CharType>
typename StringStorage<CharType>::branch_t
StringStorage<CharType>::EdgeStorage::cut(index_t at) {
  char_t ch = content_[at];

  auto first_half = content_;
  first_half.resize(at);
  auto second_half = content_.substr(at + 1);

  edge_t edge(std::move(first_half));
  content_ = std::move(second_half);
  return std::make_pair(ch, edge);
}
