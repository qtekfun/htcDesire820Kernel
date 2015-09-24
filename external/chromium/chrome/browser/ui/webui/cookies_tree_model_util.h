// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_COOKIES_TREE_MODEL_UTIL_H_
#define CHROME_BROWSER_UI_WEBUI_COOKIES_TREE_MODEL_UTIL_H_
#pragma once

#include <string>

class CookieTreeNode;
class DictionaryValue;
class ListValue;

namespace cookies_tree_model_util {

std::string GetTreeNodeId(CookieTreeNode* node);

void GetCookieTreeNodeDictionary(const CookieTreeNode& node,
                                 DictionaryValue* dict);

void GetChildNodeList(CookieTreeNode* parent, int start, int count,
                      ListValue* nodes);

CookieTreeNode* GetTreeNodeFromPath(CookieTreeNode* root,
                                    const std::string& path);

}  

#endif  
