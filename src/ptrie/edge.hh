#pragma once

namespace ptrie {

  template <class Storage>
  class EdgeStorage {
    using self_t = EdgeStorage;
    using storage_t = Storage;
    using char_t = typename storage_t::char_t;
    using string_t = typename storage_t::string_t;
    using index_t = typename storage_t::index_t;
    using branch_t = std::pair<char_t, self_t>;

  public:
    EdgeStorage(const storage_t& data, index_t off, index_t len)
      : offset_(off), length_(len), data_(data)
    {}

    index_t offset() const { return offset_; }
    index_t length() const { return length_; }
    char_t operator[](const index_t at) const { return data_[offset_ + at]; }
    string_t get_as_string() const { return data_.substr(offset_, length_); }

    branch_t cut(index_t at)
    {
      char_t ch = data_[offset_ + at];
      auto first_half = offset_;
      offset_ = offset_ + at + 1;
      length_ = length_ - at - 1;
      self_t edge(data_, first_half, at);
      return std::make_pair(ch, edge);
    }

  private:
    index_t offset_;
    index_t length_;
    const storage_t& data_;
  };

}
