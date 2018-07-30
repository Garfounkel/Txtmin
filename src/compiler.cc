#include <fstream>
#include <iostream>

#include "patricia_trie.hh"
#include "string_storage.hh"

using storage_t = StringStorage<char>;

int main(int argc, char *argv[]) {
  if (argc != 3 and (argc != 5 or argv[3] != std::string("--dot_output"))) {
    std::cerr << "Usage:" << std::endl;
    std::cerr
        << "\t./TextMiningCompiler words.txt dict.bin [--dot_ouput dot_file]"
        << std::endl;
    exit(64);
  }

  auto istream = std::ifstream(argv[1]);
  auto ostream = std::ofstream(argv[2]);

  auto ptrie = PatriciaTrie<storage_t>::read_words_file(istream);
  std::cerr << "Node number: " << ptrie.node_number_get() << std::endl;

  if (argc == 5 and (argv[3] == std::string("--dot_output"))) {
    auto dot_ostream = std::ofstream(argv[4]);
    ptrie.write_dot(dot_ostream);
  }

  return 0;
}
