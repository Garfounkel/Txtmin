#include <fstream>
#include <iostream>

#include "ptrie/patricia_trie.hh"
#include "ptrie/string_storage.hh"

using storage_t = ptrie::StringStorage;

int main(int argc, char *argv[]) {
  if (argc != 3 and (argc != 5 or argv[3] != std::string("--dot_output"))) {
    std::cerr << "Usage:" << std::endl;
    std::cerr
        << "\t./TextMiningCompiler words.txt dict.bin [--dot_ouput dot_file]"
        << std::endl;
    exit(64);
  }

  auto in = std::ifstream(argv[1]);

  std::cerr << "Building patricia trie... ";
  auto ptrie = ptrie::PatriciaTrie<storage_t>::read_words_file(in);
  std::cerr << "done." << std::endl;
  std::cerr << "Node number: " << ptrie.node_number_get() << std::endl;

  std::cerr << "Serializing... ";
  ptrie.serialize(argv[2]);
  std::cerr << "done." << std::endl;

  if (argc == 5 and (argv[3] == std::string("--dot_output"))) {
    auto dot_ostream = std::ofstream(argv[4]);
    ptrie.write_dot(dot_ostream);
  }

  return 0;
}
