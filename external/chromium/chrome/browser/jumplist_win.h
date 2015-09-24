// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_JUMPLIST_WIN_H_
#define CHROME_BROWSER_JUMPLIST_WIN_H_
#pragma once

#include <list>
#include <string>
#include <utility>
#include <vector>

#include "base/memory/ref_counted.h"
#include "chrome/browser/history/history.h"
#include "chrome/browser/sessions/tab_restore_service.h"
#include "chrome/browser/sessions/tab_restore_service_observer.h"
#include "content/browser/cancelable_request.h"

class FilePath;
class Profile;
class PageUsageData;

class ShellLinkItem : public base::RefCountedThreadSafe<ShellLinkItem> {
 public:
  ShellLinkItem() : index_(0), favicon_(false) {
  }

  const std::wstring& arguments() const { return arguments_; }
  const std::wstring& title() const { return title_; }
  const std::wstring& icon() const { return icon_; }
  int index() const { return index_; }
  scoped_refptr<RefCountedMemory> data() const { return data_; }

  void SetArguments(const std::wstring& arguments) {
    arguments_ = arguments;
  }

  void SetTitle(const std::wstring& title) {
    title_ = title;
  }

  void SetIcon(const std::wstring& icon, int index, bool favicon) {
    icon_ = icon;
    index_ = index;
    favicon_ = favicon;
  }

  void SetIconData(scoped_refptr<RefCountedMemory> data) {
    data_ = data;
  }

 private:
  friend class base::RefCountedThreadSafe<ShellLinkItem>;

  ~ShellLinkItem() {}

  std::wstring arguments_;
  std::wstring title_;
  std::wstring icon_;
  scoped_refptr<RefCountedMemory> data_;
  int index_;
  bool favicon_;

  DISALLOW_COPY_AND_ASSIGN(ShellLinkItem);
};

typedef std::vector<scoped_refptr<ShellLinkItem> > ShellLinkItemList;

class JumpList : public TabRestoreServiceObserver {
 public:
  JumpList();
  ~JumpList();

  
  
  
  bool AddObserver(Profile* profile);
  void RemoveObserver();

  
  
  
  
  
  virtual void TabRestoreServiceChanged(TabRestoreService* service);

  
  
  virtual void TabRestoreServiceDestroyed(TabRestoreService* service);

  
  
  
  
  
  
  static bool Enabled();

 protected:
  
  
  
  
  bool AddTab(const TabRestoreService::Tab* tab,
              ShellLinkItemList* list,
              size_t max_items);
  bool AddWindow(const TabRestoreService::Window* window,
                 ShellLinkItemList* list,
                 size_t max_items);

  
  
  bool StartLoadingFavicon();

  
  
  
  
  void OnSegmentUsageAvailable(CancelableRequestProvider::Handle handle,
                               std::vector<PageUsageData*>* data);

  
  
  
  
  
  
  void OnFaviconDataAvailable(HistoryService::Handle handle,
                              history::FaviconData favicon);

 private:
  
  CancelableRequestConsumer most_visited_consumer_;
  CancelableRequestConsumer favicon_consumer_;

  
  Profile* profile_;

  
  std::wstring app_id_;

  
  FilePath icon_dir_;

  
  ShellLinkItemList most_visited_pages_;

  
  ShellLinkItemList recently_closed_pages_;

  
  typedef std::pair<std::string, scoped_refptr<ShellLinkItem> > URLPair;
  std::list<URLPair> icon_urls_;
};

#endif  
