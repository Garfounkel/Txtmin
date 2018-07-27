#include <fstream>
#include <iostream>

// #include "patricia_trie.hh"
#include "trie.hh"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage:" << std::endl;
    std::cerr << "\t./TextMiningCompiler words_file_path dot_file" << std::endl;
    exit(64);
  }

  auto istream = std::ifstream(argv[1]);
  auto ostream = std::ofstream(argv[2]);

  Trie trie(istream);
  trie.write_dot(ostream);

  return 0;
}
