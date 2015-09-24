// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef _STL_DECL_MSVC_H
#define _STL_DECL_MSVC_H

#include <wchar.h>
#include <string>
#include <vector>
#include <functional>
#include <utility>
#include <set>
#include <list>
#define slist list
#include <algorithm>
#include <deque>
#include <iostream>
#include <map>
#include <queue>
#include <stack>

template <typename InputIterator, typename Size, typename OutputIterator>
std::pair<InputIterator, OutputIterator>
copy_n(InputIterator in, Size count, OutputIterator out) {
  for ( ; count > 0; --count) {
    *out = *in;
    ++out;
    ++in;
  }
  return std::make_pair(in, out);
}

template <typename T>
struct identity {
  inline const T& operator()(const T& t) const { return t; }
};

template <class _Pair>
struct select1st : public std::unary_function<_Pair, typename _Pair::first_type> {
  const typename _Pair::first_type& operator()(const _Pair& __x) const {
    return __x.first;
  }
};

template <class _Pair>
struct select2nd : public std::unary_function<_Pair, typename _Pair::second_type>
{
  const typename _Pair::second_type& operator()(const _Pair& __x) const {
    return __x.second;
  }
};


#if _MSC_VER >= 1300

namespace msvchash {
  template <typename Key>
  struct hash;

  template <class Key,
            class HashFcn = hash<Key> >
  class hash_set;

  template <class Key, class Val,
            class HashFcn = hash<Key> >
  class hash_map;

  template <class Key,
            class HashFcn = hash<Key> >
  class hash_multiset;

  template <class Key, class Val,
            class HashFcn = hash<Key> >
  class hash_multimap;
}  

using msvchash::hash_set;
using msvchash::hash_map;
using msvchash::hash;
using msvchash::hash_multimap;
using msvchash::hash_multiset;

#else
#define hash_map map
#define hash_set set
#endif

using namespace std;

#endif   
