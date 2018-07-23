#include "patricia_trie.hh"

PatriciaTrie::PatriciaTrie(std::istream& file)
  : root_(std::make_shared<node_t>(this))
{
  while (file.good())
  {
    string_t word;
    unsigned freq;
    file >> word >> freq;

    insert(word, freq);
  }
}

void PatriciaTrie::insert(const string_t& word, const unsigned freq)
{
  auto it = std::begin(word);
  auto cursor = Cursor(&root_);

  for (; it != std::end(word); it++)
  {
    if (!cursor.next(*it))
      break;
  }

  if (cursor.is_node())
  { // Cursor is on a node
    auto& node = cursor.get_as_node();
    if (it == std::end(word))
      node.add_freq(freq);
    else
      create_edge(it, std::end(word), std::make_shared<node_t>(this, freq));
  }
  else
  { // Cursor is on an edge
    auto& edge = cursor.get_as_edge();
    if (it == std::end(word))
    {
      edge.length_set(edge.length_get() - cursor.index_get() + 1);

      // TODO: continue below
      auto first_char = char_table_[/*offset + */]

      edge.next_.children_.emplace_back(first_char, ...)
    }
    else
    {

    }
  }
}

bool PatriciaTrie::Cursor::next(char_t c)
{
  if (std::holds_alternative<Node*>(current_))
  { // Node
    auto cur = std::get<Node*>(current_);
    auto opt = cur->get_edge(c);
    if (!opt)
      return false;
    current_ = *opt;
    index_ = 1;
  }
  else
  { // Edge
    auto cur = std::get<Edge*>(current_);
    if (cur->get_char_at(index_) != c)
    {
      return false;
    }
    if (index_ == cur->length_get())
    {
      index_ = 0;
      current_ = cur->next_get().get();
    }
    else
    {
      index_++;
    }
  }
  return true;
}

bool PatriciaTrie::Cursor::is_node() const
{
  return std::holds_alternative<Node*>(current_);
}

void PatriciaTrie::Node::create_edge(string_t::const_iterator start,
                                     string_t::const_iterator end,
                                     node_ptr_t next_node)
{
  auto offset = trie_get()->char_table_.length();
  trie_get()->char_table_.append(start + 1, end);
  children_.emplace_back(trie_get(), *start, offset, end - start, next_node);
}

bool PatriciaTrie::Node::is_word()
{
  return freq_ > 0;
}

PatriciaTrie::char_t PatriciaTrie::Edge::get_char_at(index_t index) const
{
  if (index == 0)
    return first_char_;

  assert(index <= length_);

  return trie_get()->char_table_[offset_ + index - 1];
}

std::optional<PatriciaTrie::Edge*> PatriciaTrie::Node::get_edge(char_t c) const
{
  for (Edge& child : children_)
  {
    if (child.get_char_at(0) == c)
      return std::make_optional(&child);
  }

  return std::nullopt;
}
