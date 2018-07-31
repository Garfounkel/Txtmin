#pragma once

#include <fstream>
#include <istream>
#include <ostream>
#include <string>
#include <utility>

#include "edge.hh"
#include "types.hh"

namespace ptrie {

  class StringStorage {
  public:
    using self_t = StringStorage;
    using edge_t = EdgeStorage<self_t>;
    using char_t = typename types::char_type;
    using string_t = typename types::string_type;
    using index_t = typename types::index_type;

    static self_t deserialize(const std::string& path, index_t len, index_t off)
    {
      std::ifstream in(path, std::ios::in | std::ios::binary);
      in.seekg(off);
      self_t store;
      store.data_.reserve(len);
      std::copy_n(std::istreambuf_iterator(in), len, std::back_inserter(store.data_));
      return store;
    }

    edge_t empty_edge() const { return edge_t(*this, 0, 0); }

    edge_t new_edge(const string_t &content)
    {
      if (content.empty())
        return empty_edge();
      auto off = data_.length();
      auto len = content.length();
      data_.append(content);
      return edge_t(*this, off, len);
    }

    char operator[](const index_t at) const { return data_[at]; }

    string_t substr(index_t off, index_t len) const
    { return data_.substr(off, len); }

    std::ostream& serialize(std::ostream& out) const
    {
      index_t len = data_.length() * sizeof(char_t);
      out.write(reinterpret_cast<char*>(&len), sizeof(len));
      out.write(reinterpret_cast<const char*>(data_.data()), len);
      return out;
    }

  private:
    string_t data_;
  };

}
