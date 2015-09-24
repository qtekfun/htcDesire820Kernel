// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_EDITOR_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_EDITOR_H_
#pragma once

#include <utility>
#include <vector>

#include "base/string16.h"
#include "ui/gfx/native_widget_types.h"

class BookmarkNode;
class GURL;
class Profile;

class BookmarkEditor {
 public:
  
  enum Configuration {
    SHOW_TREE,
    NO_TREE
  };

  
  struct EditDetails {
    enum Type {
      
      
      EXISTING_NODE,

      
      
      NEW_URL,

      
      
      
      NEW_FOLDER
    };

    EditDetails();
    explicit EditDetails(const BookmarkNode* node);
    ~EditDetails();

    
    Type type;

    
    const BookmarkNode* existing_node;

    
    
    std::vector<std::pair<GURL, string16> > urls;
  };

  
  
  
  
  
  
  static void Show(gfx::NativeWindow parent_window,
                   Profile* profile,
                   const BookmarkNode* parent,
                   const EditDetails& details,
                   Configuration configuration);
};

#endif  
