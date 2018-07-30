#include <fstream>
#include <iostream>

#include "patricia_trie.hh"
#include "string_storage.hh"

using storage_t = StringStorage<char>;

void print_results(std::ostream &out,
                   const PatriciaTrie<storage_t>::results_t &results) {
  out << "[";
  for (auto res = std::begin(results); res < std::end(results); res++) {
    out << "{"
        << "\"word\":\"" << res->word
        << "\","
           "\"freq\":"
        << res->freq
        << ","
           "\"distance\":"
        << res->distance << "}";
    if (res != std::end(results) - 1)
      out << ",";
  }
  out << "]" << std::endl;
}

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
  auto results = ptrie.search_dist("br", 5);
  print_results(std::cout, results);
  ptrie.write_dot(ostream);

  return 0;
}
