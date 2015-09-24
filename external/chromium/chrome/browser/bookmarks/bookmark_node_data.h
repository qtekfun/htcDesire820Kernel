// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_NODE_DATA_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_NODE_DATA_H_
#pragma once

#include <vector>

#include "base/file_path.h"
#include "base/string16.h"
#include "googleurl/src/gurl.h"

#if defined(TOOLKIT_VIEWS)
#include "ui/base/dragdrop/os_exchange_data.h"
#endif

class BookmarkNode;
class Pickle;
class Profile;


struct BookmarkNodeData {
  
  struct Element {
    Element();
    explicit Element(const BookmarkNode* node);
    ~Element();

    
    bool is_url;

    
    GURL url;

    
    string16 title;

    
    std::vector<Element> children;

    int64 get_id() {
      return id_;
    }
   private:
    friend struct BookmarkNodeData;

    
    void WriteToPickle(Pickle* pickle) const;
    bool ReadFromPickle(Pickle* pickle, void** iterator);

    
    int64 id_;
  };

  BookmarkNodeData();

#if defined(TOOLKIT_VIEWS)
  static ui::OSExchangeData::CustomFormat GetBookmarkCustomFormat();
#endif

  
  explicit BookmarkNodeData(const BookmarkNode* node);
  explicit BookmarkNodeData(const std::vector<const BookmarkNode*>& nodes);

  ~BookmarkNodeData();

  
  bool ReadFromVector(const std::vector<const BookmarkNode*>& nodes);

  
  bool ReadFromTuple(const GURL& url, const string16& title);

  
  void WriteToClipboard(Profile* profile) const;

  
  // written via WriteToClipboard but will also attempt to read a plain
  
  bool ReadFromClipboard();
#if defined(OS_MACOSX)
  
  
  bool ReadFromDragClipboard();
#endif

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

  
  static const char* kClipboardFormatString;

  static bool ClipboardContainsBookmarks();

 private:
  
  FilePath::StringType profile_path_;
};

#endif  
