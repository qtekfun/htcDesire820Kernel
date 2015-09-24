// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_ANDROID_FORWARDER2_UTIL_H_
#define TOOLS_ANDROID_FORWARDER2_UTIL_H_

#include "base/logging.h"

namespace forwarder2 {

template <typename Map, typename K>
bool DeleteRefCountedValueInMap(const K& key, Map* map) {
  const typename Map::iterator it = map->find(key);
  if (it == map->end())
    return false;
  DeleteRefCountedValueInMapFromIterator(it, map);
  return true;
}

template <typename Map, typename Iterator>
void DeleteRefCountedValueInMapFromIterator(Iterator it, Map* map) {
  DCHECK(it != map->end());
  const typename Map::value_type::second_type shared_ptr_copy = it->second;
  map->erase(it);
}

}  

#endif  
