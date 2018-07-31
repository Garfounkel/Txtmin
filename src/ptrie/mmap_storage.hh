#pragma once

#include <ostream>
#include <string>
#include <utility>

extern "C" {
  #include <sys/mman.h>
  #include <fcntl.h>
  #include <unistd.h>
}

#include "edge.hh"
#include "types.hh"

namespace ptrie {

  class MmapStorage {
  public:
    using self_t = MmapStorage;
    using edge_t = EdgeStorage<self_t>;
    using char_t = typename types::char_type;
    using string_t = typename types::string_type;
    using index_t = typename types::index_type;

    static self_t deserialize(const std::string& path, index_t len, index_t off)
    {
      int fd = open(path.c_str(), O_RDONLY);
      if (fd == -1)
        return self_t(nullptr, 0);
      auto data_ = mmap(nullptr, len, PROT_READ, MAP_SHARED, fd, off);
      if (data_ == MAP_FAILED) {
        data_ = nullptr;
        len = 0;
      }
      close(fd);
      return self_t(static_cast<const char*>(data_), len);
    }

    ~MmapStorage()
    {
      munmap(const_cast<char*>(data_), len_);
      data_ = nullptr;
      len_ = 0;
    }

    edge_t empty_edge() const { return edge_t(*this, 0, 0); }

    // remove me
    //edge_t new_edge(const string_t& content) { assert(false); return edge_t(*this, 0, 0); }

    char operator[](const index_t at) const { return data_[at]; }

    string_t substr(index_t off, index_t len) const
    { return std::string(data_ + off, data_ + off + len); }

    std::ostream& serialize(std::ostream& out) const
    {
      out.write(reinterpret_cast<const char*>(&len_), sizeof(len_));
      out.write(data_, len_);
      return out;
    }

  private:
    MmapStorage(const char* mmapped_ptr, index_t len)
      : len_(len), data_(mmapped_ptr)
    {}

    index_t len_;
    const char* data_;
  };

}
