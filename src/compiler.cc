#include <fstream>
#include <iostream>

// #include "patricia_trie.hh"
#include "trie.hh"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage:" << std::endl;
    std::cerr << "\t./TextMiningCompiler words_file_path" << std::endl;
    exit(64);
  }

  auto stream = std::ifstream(argv[1]);

  // PatriciaTrie trie(stream);
  Trie trie(stream);
  return 0;
}
