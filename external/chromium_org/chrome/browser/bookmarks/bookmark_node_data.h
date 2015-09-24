// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_NODE_DATA_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_NODE_DATA_H_

#include <vector>

#include "base/files/file_path.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "chrome/browser/bookmarks/bookmark_model.h"
#include "ui/base/clipboard/clipboard_types.h"

#include "url/gurl.h"
#if defined(TOOLKIT_VIEWS)
#include "ui/base/dragdrop/os_exchange_data.h"
#endif

class Pickle;
class PickleIterator;
class Profile;


struct BookmarkNodeData {
  
  struct Element {
    Element();
    explicit Element(const BookmarkNode* node);
    ~Element();

    
    bool is_url;

    
    GURL url;

    
    base::string16 title;

    
    base::Time date_added;

    
    base::Time date_folder_modified;

    
    std::vector<Element> children;

    
    BookmarkNode::MetaInfoMap meta_info_map;

    int64 id() const { return id_; }

   private:
    friend struct BookmarkNodeData;

    
    void WriteToPickle(Pickle* pickle) const;
    bool ReadFromPickle(Pickle* pickle, PickleIterator* iterator);

    
    int64 id_;
  };

  
  static const char* kClipboardFormatString;

  BookmarkNodeData();

  
  explicit BookmarkNodeData(const BookmarkNode* node);
  explicit BookmarkNodeData(const std::vector<const BookmarkNode*>& nodes);

  ~BookmarkNodeData();

#if defined(TOOLKIT_VIEWS)
  static const ui::OSExchangeData::CustomFormat& GetBookmarkCustomFormat();
#endif

  static bool ClipboardContainsBookmarks();

  
  bool ReadFromVector(const std::vector<const BookmarkNode*>& nodes);

  
  bool ReadFromTuple(const GURL& url, const base::string16& title);

  
  void WriteToClipboard(ui::ClipboardType type);

  // Reads bookmarks from the specified clipboard. Prefers data written via
  
  bool ReadFromClipboard(ui::ClipboardType type);

#if defined(TOOLKIT_VIEWS)
  
  // the URL and title are written to the clipboard in a format other apps can
  
  
  
  void Write(Profile* profile, ui::OSExchangeData* data) const;

  
  bool Read(const ui::OSExchangeData& data);
#endif

  
  void WriteToPickle(Profile* profile, Pickle* pickle) const;

  
  bool ReadFromPickle(Pickle* pickle);

  
  
  
  
  std::vector<const BookmarkNode*> GetNodes(Profile* profile) const;

  
  
  const BookmarkNode* GetFirstNode(Profile* profile) const;

  
  bool is_valid() const { return !elements.empty(); }

  
  bool has_single_url() const { return is_valid() && elements[0].is_url; }

  
  size_t size() const { return elements.size(); }

  
  void Clear();

  
  
  
  void SetOriginatingProfile(Profile* profile);

  
  bool IsFromProfile(Profile* profile) const;

  // The actual elements written to the clipboard.
  std::vector<Element> elements;

 private:
  
  base::FilePath profile_path_;
};

#endif  
