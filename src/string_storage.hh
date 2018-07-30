#pragma once

#include <ostream>
#include <string>

template <typename CharType = char> class StringStorage {
  class EdgeStorage;

public:
  using edge_t = EdgeStorage;
  using char_t = CharType;
  using string_t = std::basic_string<char_t>;
  using index_t = std::size_t;
  using branch_t = std::pair<char_t, edge_t>;

  edge_t new_edge(const string_t &content);
  std::ostream& dump(std::ostream& out) const;

private:
  class EdgeStorage {
  public:
    EdgeStorage(string_t& data, index_t off, index_t len)
      : offset_(off), length_(len), data_(data)
    {}
    branch_t cut(index_t at);
    index_t offset() const { return offset_; }
    index_t length() const { return length_; }
    char_t &operator[](index_t at) { return data_[offset_ + at]; }
    char_t operator[](const index_t at) const { return data_[offset_ + at]; }
    string_t get_as_string() const { return data_.substr(offset_, length_); }

  private:
    index_t offset_;
    index_t length_;
    string_t& data_;
  };

  string_t data_;
};

#include "string_storage.hxx"
