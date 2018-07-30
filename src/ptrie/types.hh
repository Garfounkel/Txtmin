#pragma once

#include <string>

namespace ptrie {
  struct types {
    using char_type = char;
    using string_type = std::basic_string<char_type>;
    using index_type = std::size_t;
  };
}
