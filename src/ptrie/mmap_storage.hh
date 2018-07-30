#pragma once

#include <ostream>
#include <string>
#include <utility>

extern "C" {
  #include <sys/mman.h>
  #include <fcntl.h>
}

#include "edge.hh"
#include "types.hh"

namespace ptrie {

  class MmapStorage {
  public:
    using self_t = MmapStorage;
    using edge_t = EdgeStorage<self_t>;
    using char_t = typename storage_types::storage_char_t;
    using string_t = typename storage_types::storage_string_t;
    using index_t = typename storage_types::storage_index_t;

    MmapStorage(const string_t& path, index_t len, index_t off = sizeof(index_t))
      : data_(nullptr), len_(len)
    {
      int fd = open(path, O_RDONLY);
      if (fd == -1)
        return;
      data_ = mmap(nullptr, len, prot, flags, fd, off);
      if (data_ == MAP_FAILED)
        data_ = nullptr;
      close(fd);
    }

    ~MmapStorage() { munmap(data_, len_); }

    edge_t new_edge(const string_t& content) { assert(false); return edge_t(*this, 0, 0); }

    char operator[](const index_t at) const { return data_[at]; }

    string_t substr(index_t off, index_t len) const
    { return std::string(data_ + off, data_ + off + len); }

    std::ostream& dump(std::ostream& out) const;
    {
      out.write(reinterpret_cast<char*>(&len_), sizeof(len_));
      out.write(data_, len);
      return out;
    }

  private:
    const index_t len_;
    const char* data_;
  };

}
