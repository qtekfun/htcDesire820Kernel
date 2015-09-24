// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_MODELS_TREE_NODE_ITERATOR_H_
#define UI_BASE_MODELS_TREE_NODE_ITERATOR_H_

#include <stack>

#include "base/basictypes.h"
#include "base/logging.h"

namespace ui {

template <class NodeType>
class TreeNodeIterator {
 public:
  
  
  
  
  TreeNodeIterator(NodeType* node, bool (*prune)(NodeType*))
      : prune_(prune) {
    int index = 0;

    
    
    
    
    for (; index < node->child_count(); ++index)
      if (!prune || !prune(node->GetChild(index)))
        break;

    if (index < node->child_count())
      positions_.push(Position<NodeType>(node, index));
  }

  explicit TreeNodeIterator(NodeType* node) : prune_(NULL) {
    if (!node->empty())
      positions_.push(Position<NodeType>(node, 0));
  }

  
  bool has_next() const { return !positions_.empty(); }

  
  NodeType* Next() {
    if (!has_next()) {
      NOTREACHED();
      return NULL;
    }

    
    NodeType* result = positions_.top().node->GetChild(positions_.top().index);

    
    positions_.top().index++;

    
    positions_.push(Position<NodeType>(result, 0));

    
    
    
    
    while (!positions_.empty()) {
      if (positions_.top().index >= positions_.top().node->child_count())
        positions_.pop(); 
      else if (prune_ &&
          prune_(positions_.top().node->GetChild(positions_.top().index)))
        positions_.top().index++;  
      else
        break;  
    }

    return result;
  }

 private:
  template <class PositionNodeType>
  struct Position {
    Position(PositionNodeType* node, int index) : node(node), index(index) {}
    Position() : node(NULL), index(-1) {}

    PositionNodeType* node;
    int index;
  };

  std::stack<Position<NodeType> > positions_;
  bool (*prune_)(NodeType*);

  DISALLOW_COPY_AND_ASSIGN(TreeNodeIterator);
};

}  

#endif  
