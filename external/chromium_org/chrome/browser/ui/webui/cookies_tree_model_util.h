// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_COOKIES_TREE_MODEL_UTIL_H_
#define CHROME_BROWSER_UI_WEBUI_COOKIES_TREE_MODEL_UTIL_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/id_map.h"

class CookieTreeNode;

namespace base {
class DictionaryValue;
class ListValue;
}

class CookiesTreeModelUtil {
 public:
  CookiesTreeModelUtil();
  ~CookiesTreeModelUtil();

  
  std::string GetTreeNodeId(const CookieTreeNode* node);

  
  void GetChildNodeList(const CookieTreeNode* parent,
                        int start,
                        int count,
                        base::ListValue* nodes);

  
  
  
  const CookieTreeNode* GetTreeNodeFromPath(const CookieTreeNode* root,
                                            const std::string& path);

 private:
  typedef IDMap<const CookieTreeNode> CookiesTreeNodeIdMap;
  typedef std::map<const CookieTreeNode*, int32> CookieTreeNodeMap;

  
  
  
  bool GetCookieTreeNodeDictionary(const CookieTreeNode& node,
                                   base::DictionaryValue* dict);

  
  CookiesTreeNodeIdMap id_map_;

  
  CookieTreeNodeMap node_map_;

  DISALLOW_COPY_AND_ASSIGN(CookiesTreeModelUtil);
};

#endif  
