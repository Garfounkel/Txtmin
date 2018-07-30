#include <fstream>
#include <iostream>

#include "patricia_trie.hh"
#include "string_storage.hh"

using storage_t = StringStorage<char>;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage:" << std::endl;
    std::cerr << "\t./TextMiningCompiler words_file_path dot_file" << std::endl;
    exit(64);
  }

  auto istream = std::ifstream(argv[1]);
  auto ostream = std::ofstream(argv[2]);

  auto ptrie = PatriciaTrie<storage_t>::read_words_file(istream);
  std::cout << "Node number: " << ptrie.node_number_get() << std::endl;
//  ptrie.write_dot(ostream);
  ptrie.serialize(argv[2]);
  return 0;
}
