#pragma once

#include <string>
#include <cstdint>

template <typename CharType = char> class StringStorage {
  class EdgeStorage;

public:
  using edge_t = EdgeStorage;
  using char_t = CharType;
  using string_t = std::basic_string<char_t>;
  using index_t = std::size_t;
  using branch_t = std::pair<char_t, edge_t>;

  edge_t new_edge(const string_t &content) { return EdgeStorage(content); }

private:
  class EdgeStorage {
  public:
    EdgeStorage(const string_t &content) : content_(content) {}
    branch_t cut(index_t at);
    index_t length() const { return content_.length(); }
    char_t &operator[](index_t at) { return content_[at]; }
    char_t operator[](const index_t at) const { return content_[at]; }
    string_t const &content_get() const { return content_; }

  private:
    string_t content_;
  };
};

#include "string_storage.hxx"
