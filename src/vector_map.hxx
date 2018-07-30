#pragma once

#include <stdexcept>

template <class K, class T, class C>
typename VectorMap<K,T,C>::mapped_type&
VectorMap<K,T,C>::operator[](const key_type& key)
{
  auto it = std::lower_bound(begin(), end(), key, comp_);
  if (it == end() || not equiv(it->first, key)) {
    it = vect_.emplace(it, key, mapped_type());
  }
  return it->second;
}

template <class K, class T, class C>
const typename VectorMap<K,T,C>::mapped_type&
VectorMap<K,T,C>::at(const key_type& key) const
{
  auto it = std::lower_bound(cbegin(), cend(), key, comp_);
  if (it == cend() || not equiv(it->first, key)) {
    throw std::out_of_range("VectorMap: at: key not found");
  }
  return it->second;
}

template <class K, class T, class C>
typename VectorMap<K,T,C>::mapped_type&
VectorMap<K,T,C>::at(const key_type& key)
{
  return const_cast<mapped_type&>(static_cast<const map&>(*this).at(key));
}

template <class K, class T, class C>
typename VectorMap<K,T,C>::size_type
VectorMap<K,T,C>::count(const key_type& key) const
{
  auto it = std::lower_bound(begin(), end(), key, comp_);
  return it != end() && equiv(it->first, key) ? 1 : 0;
}

template <class K, class T, class C>
typename VectorMap<K,T,C>::iterator
VectorMap<K,T,C>::find(const key_type& key)
{
  auto it = std::lower_bound(begin(), end(), key, comp_);
  return it != end() && equiv(it->first, key) ? it : end();
}

template <class K, class T, class C>
typename VectorMap<K,T,C>::const_iterator
VectorMap<K,T,C>::find(const key_type& key) const
{
  auto it = std::lower_bound(cbegin(), cend(), key, comp_);
  return it != cend() && equiv(it->first, key) ? it : cend();
}

template <class K, class T, class C>
std::pair<typename VectorMap<K,T,C>::iterator, bool>
VectorMap<K,T,C>::insert(const value_type& val)
{
  auto it = std::lower_bound(begin(), end(), val.first, comp_);
  if (it != end() && equiv(*it, val)) {
    return std::make_pair(it, false);
  }
  it = vect_.insert(it, val);
  return std::make_pair(it, true);
}
