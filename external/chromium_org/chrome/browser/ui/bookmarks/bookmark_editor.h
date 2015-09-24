// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BOOKMARKS_BOOKMARK_EDITOR_H_
#define CHROME_BROWSER_UI_BOOKMARKS_BOOKMARK_EDITOR_H_

#include <utility>
#include <vector>

#include "base/strings/string16.h"
#include "chrome/browser/bookmarks/bookmark_model.h"
#include "ui/gfx/native_widget_types.h"

class GURL;
class Profile;

class BookmarkEditor {
 public:
  
  enum Configuration {
    
    
    SHOW_TREE,
    NO_TREE
  };

  
  class EditDetails {
   public:
    
    BookmarkNode::Type GetNodeType() const;

    
    
    int GetWindowTitleId() const;

    
    static EditDetails EditNode(const BookmarkNode* node);

    
    
    static EditDetails AddNodeInFolder(const BookmarkNode* parent_node,
                                       int index,
                                       const GURL& url,
                                       const base::string16& title);

    
    
    static EditDetails AddFolder(const BookmarkNode* parent_node,
                                 int index);

    enum Type {
      
      
      EXISTING_NODE,

      
      
      NEW_URL,

      
      
      
      NEW_FOLDER
    };

    ~EditDetails();

    
    const Type type;

    
    const BookmarkNode* existing_node;

    
    
    const BookmarkNode* parent_node;

    
    
    int index;

    
    GURL url;
    base::string16 title;

    
    
    std::vector<std::pair<GURL, base::string16> > urls;

   private:
    explicit EditDetails(Type node_type);
  };

  
  
  
  static void Show(gfx::NativeWindow parent_window,
                   Profile* profile,
                   const EditDetails& details,
                   Configuration configuration);

  
  
  
  
  static const BookmarkNode* ApplyEditsWithNoFolderChange(
      BookmarkModel* model,
      const BookmarkNode* parent,
      const EditDetails& details,
      const base::string16& new_title,
      const GURL& new_url);

  
  
  
  
  static const BookmarkNode* ApplyEditsWithPossibleFolderChange(
      BookmarkModel* model,
      const BookmarkNode* new_parent,
      const EditDetails& details,
      const base::string16& new_title,
      const GURL& new_url);
};

#endif  
