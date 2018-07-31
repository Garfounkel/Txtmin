cmake_minimum_required (VERSION 2.6)
project (TMLN)

include_directories(src)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -std=c++17 -g")

set(SRC
  # src/trie.cc
)

add_executable(TextMiningCompiler src/compiler.cc ${SRC})
add_executable(TextMiningApp      src/app.cc      ${SRC})
