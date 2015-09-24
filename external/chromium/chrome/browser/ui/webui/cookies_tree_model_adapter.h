// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_COOKIES_TREE_MODEL_ADAPTER_H_
#define CHROME_BROWSER_UI_WEBUI_COOKIES_TREE_MODEL_ADAPTER_H_
#pragma once

#include "chrome/browser/cookies_tree_model.h"

class ListValue;
class Value;
class WebUI;

class CookiesTreeModelAdapter : public CookiesTreeModel::Observer {
 public:
  CookiesTreeModelAdapter();
  virtual ~CookiesTreeModelAdapter();

  
  void Init(WebUI* web_ui);

  
  
  void Bind(const std::string& tree_id, CookiesTreeModel* model);

 private:
  
  virtual void TreeNodesAdded(ui::TreeModel* model,
                              ui::TreeModelNode* parent,
                              int start,
                              int count);
  virtual void TreeNodesRemoved(ui::TreeModel* model,
                                ui::TreeModelNode* parent,
                                int start,
                                int count);
  virtual void TreeNodeChanged(ui::TreeModel* model, ui::TreeModelNode* node) {}
  virtual void TreeModelBeginBatch(CookiesTreeModel* model);
  virtual void TreeModelEndBatch(CookiesTreeModel* model);

  
  
  void RequestChildren(const ListValue* args);

  
  
  void SendChildren(CookieTreeNode* parent);

  
  
  Value* GetTreeNodeId(CookieTreeNode* node);

  
  WebUI* web_ui_;

  
  std::string tree_id_;

  
  CookiesTreeModel* model_;

  
  bool batch_update_;

  DISALLOW_COPY_AND_ASSIGN(CookiesTreeModelAdapter);
};

#endif  
