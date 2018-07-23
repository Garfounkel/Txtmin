cmake_minimum_required (VERSION 2.6)
project (TMLN)

set(SRC
  src/patricia_trie.cc
)

add_executable(TextMiningCompiler src/compiler.cc ${SRC})
add_executable(TextMiningApp      src/app.cc      ${SRC})
