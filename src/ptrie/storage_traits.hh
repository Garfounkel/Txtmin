#pragma once

namespace ptrie {
  class StringStorage;

  template <class Storage>
  struct StorageTraits {
    enum { read_only = true };
  };

  template <>
  struct StorageTraits<StringStorage> {
    enum { read_only = false };
  };

}
