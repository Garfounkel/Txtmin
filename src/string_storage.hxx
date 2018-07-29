#pragma once

template <typename CharType>
typename StringStorage<CharType>::branch_t
StringStorage<CharType>::EdgeStorage::cut(index_t at) {
  char_t ch = content_[at];
  edge_t ed = content_.substr(at + 1);
  content_.resize(at);
  std::swap(content_, ed);
  return std::make_pair(ch, ed);
}
