#pragma once

#include <utility>

template <typename C>
typename StringStorage<C>::edge_t
StringStorage<C>::new_edge(const string_t& content)
{
  if (content.empty())
    return edge_t(data_, 0, 0);
  auto off = data_.length();
  auto len = content.length();
  data_.append(content);
  return edge_t(data_, off, len);
}

template <typename C>
std::ostream& StringStorage<C>::dump(std::ostream& out) const
{
  index_t len = data_.length() * sizeof(char_t);
  out.write(reinterpret_cast<char*>(&len), sizeof(len));
  out.write(reinterpret_cast<const char*>(data_.data()), len);
  return out;
}

template <typename C>
typename StringStorage<C>::branch_t
StringStorage<C>::EdgeStorage::cut(index_t at) {
  char_t ch = data_[offset_ + at];
  auto first_half = offset_;
  offset_ = offset_ + at + 1;
  length_ = length_ - at - 1;
  edge_t edge(data_, first_half, at);
  return std::make_pair(ch, edge);
}
