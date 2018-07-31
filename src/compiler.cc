#include <fstream>
#include <iostream>

#include "ptrie/patricia_trie.hh"
#include "ptrie/string_storage.hh"
#include "ptrie/mmap_storage.hh"

using storage_t = ptrie::StringStorage;
using ptrie_t = ptrie::PatriciaTrie<storage_t>;

ptrie_t ptrie_from_words_file(std::istream &words) {
    ptrie_t ptrie = ptrie_t(storage_t());
    while (words.good()) {
      ptrie_t::string_t word;
      ptrie_t::freq_t freq;
      words >> word >> freq;
      ptrie.insert(word, freq);
    }
    return ptrie;
}

int main(int argc, char *argv[]) {
  if (argc != 3 and (argc != 5 or argv[3] != std::string("--dot_output"))) {
    std::cerr << "Usage:" << std::endl;
    std::cerr
        << "\t./TextMiningCompiler words.txt dict.bin [--dot_ouput dot_file]"
        << std::endl;
    exit(64);
  }
/*
  auto in = std::ifstream(argv[1]);

  std::cerr << "Building patricia trie... ";
  auto ptrie = ptrie_from_words_file(in);
  std::cerr << "done." << std::endl;
  std::cerr << "Node number: " << ptrie.node_number_get() << std::endl;

  std::cerr << "Serializing... ";
  ptrie.serialize(argv[2]);
  std::cerr << "done." << std::endl;
*/
  auto ptrie = ptrie::PatriciaTrie<storage_t>::deserialize(argv[1]);
  if (argc == 5 and (argv[3] == std::string("--dot_output"))) {
    auto dot_ostream = std::ofstream(argv[4]);
    ptrie.write_dot(dot_ostream);
  }

  return 0;
}
