#include "string_storage.hh"

typename StringStorage::branch_t StringStorage::EdgeStorage::cut(index_t at)
{
  char_t ch = content_[at];
  edge_t ed = content_.substr(at + 1);
  content_.resize(at);
  return std::make_pair(ch, ed);
}
