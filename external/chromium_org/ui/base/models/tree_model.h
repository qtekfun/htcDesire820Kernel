// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_MODELS_TREE_MODEL_H_
#define UI_BASE_MODELS_TREE_MODEL_H_

#include <vector>

#include "base/strings/string16.h"
#include "ui/base/ui_export.h"

namespace gfx {
class ImageSkia;
}

namespace ui {

class TreeModel;


class TreeModelNode {
 public:
  
  virtual const base::string16& GetTitle() const = 0;

 protected:
  virtual ~TreeModelNode() {}
};

class UI_EXPORT TreeModelObserver {
 public:
  
  virtual void TreeNodesAdded(TreeModel* model,
                              TreeModelNode* parent,
                              int start,
                              int count) = 0;

  
  virtual void TreeNodesRemoved(TreeModel* model,
                                TreeModelNode* parent,
                                int start,
                                int count) = 0;

  
  virtual void TreeNodeChanged(TreeModel* model, TreeModelNode* node) = 0;

 protected:
  virtual ~TreeModelObserver() {}
};


class UI_EXPORT TreeModel {
 public:
  
  
  virtual TreeModelNode* GetRoot() = 0;

  
  virtual int GetChildCount(TreeModelNode* parent) = 0;

  
  virtual TreeModelNode* GetChild(TreeModelNode* parent, int index) = 0;

  
  virtual int GetIndexOf(TreeModelNode* parent, TreeModelNode* child) = 0;

  
  virtual TreeModelNode* GetParent(TreeModelNode* node) = 0;

  
  virtual void AddObserver(TreeModelObserver* observer) = 0;

  
  virtual void RemoveObserver(TreeModelObserver* observer) = 0;

  
  
  virtual void SetTitle(TreeModelNode* node, const base::string16& title);

  
  
  virtual void GetIcons(std::vector<gfx::ImageSkia>* icons) {}

  
  
  
  virtual int GetIconIndex(TreeModelNode* node);

 protected:
  virtual ~TreeModel() {}
};

}  

#endif  
