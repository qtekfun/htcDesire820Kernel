// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_TAGGED_LIST_H_
#define CONTENT_RENDERER_MEDIA_TAGGED_LIST_H_

#include <algorithm>
#include <list>

#include "base/logging.h"
#include "base/memory/ref_counted.h"

namespace content {

template <class ItemType>
class TaggedList {
 public:
  typedef std::list<scoped_refptr<ItemType> > ItemList;

  TaggedList() {}

  void AddAndTag(ItemType* item) {
    items_.push_back(item);
    tagged_items_.push_back(item);
  }

  void TagAll() {
    tagged_items_ = items_;
  }

  const ItemList& Items() const {
    return items_;
  }

  
  
  
  void RetrieveAndClearTags(ItemList* dest) {
    DCHECK(dest->empty());
    dest->swap(tagged_items_);
  }

  
  
  template <class UnaryPredicate>
  scoped_refptr<ItemType> Remove(UnaryPredicate predicate) {
    tagged_items_.remove_if(predicate);

    typename ItemList::iterator it = std::find_if(
        items_.begin(), items_.end(), predicate);
    if (it != items_.end()) {
      scoped_refptr<ItemType> removed_item = *it;
      items_.erase(it);
      return removed_item;
    }

    return NULL;
  }

  template <class UnaryPredicate>
  bool Contains(UnaryPredicate predicate) const {
    return std::find_if(items_.begin(), items_.end(), predicate) !=
        items_.end();
  }

  void Clear() {
    items_.clear();
    tagged_items_.clear();
  }

  bool IsEmpty() const {
    bool is_empty = items_.empty();
    DCHECK(!is_empty || tagged_items_.empty());
    return is_empty;
  }

 private:
  ItemList items_;
  ItemList tagged_items_;

  DISALLOW_COPY_AND_ASSIGN(TaggedList);
};

}  

#endif  
