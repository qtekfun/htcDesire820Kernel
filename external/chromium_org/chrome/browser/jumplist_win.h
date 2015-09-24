// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_JUMPLIST_WIN_H_
#define CHROME_BROWSER_JUMPLIST_WIN_H_

#include <list>
#include <string>
#include <utility>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/history/history_service.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/sessions/tab_restore_service.h"
#include "chrome/browser/sessions/tab_restore_service_observer.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "content/public/browser/browser_thread.h"
#include "third_party/skia/include/core/SkBitmap.h"

namespace base {
class FilePath;
}

namespace chrome {
struct FaviconImageResult;
}

namespace content {
class NotificationRegistrar;
}

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
  const SkBitmap& data() const { return data_; }

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

  void SetIconData(const SkBitmap& data) {
    data_ = data;
  }

 private:
  friend class base::RefCountedThreadSafe<ShellLinkItem>;

  ~ShellLinkItem() {}

  std::wstring arguments_;
  std::wstring title_;
  std::wstring icon_;
  SkBitmap data_;
  int index_;
  bool favicon_;

  DISALLOW_COPY_AND_ASSIGN(ShellLinkItem);
};

typedef std::vector<scoped_refptr<ShellLinkItem> > ShellLinkItemList;

class JumpList : public TabRestoreServiceObserver,
                 public content::NotificationObserver,
                 public base::RefCountedThreadSafe<
                     JumpList, content::BrowserThread::DeleteOnUIThread> {
 public:
  JumpList();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details);

  
  
  
  bool AddObserver(Profile* profile);
  void RemoveObserver();

  
  
  
  
  
  virtual void TabRestoreServiceChanged(TabRestoreService* service);

  
  
  virtual void TabRestoreServiceDestroyed(TabRestoreService* service);

  
  void CancelPendingUpdate();

  
  
  
  void Terminate();

  
  
  
  
  
  
  static bool Enabled();

 protected:
  
  
  
  
  bool AddTab(const TabRestoreService::Tab* tab,
              ShellLinkItemList* list,
              size_t max_items);
  void AddWindow(const TabRestoreService::Window* window,
                 ShellLinkItemList* list,
                 size_t max_items);

  
  
  
  
  void StartLoadingFavicon();

  
  
  
  
  void OnSegmentUsageAvailable(CancelableRequestProvider::Handle handle,
                               std::vector<PageUsageData*>* data);

  
  
  
  
  void OnFaviconDataAvailable(const chrome::FaviconImageResult& image_result);

  
  
  
  
  void OnMostVisitedURLsAvailable(
      const history::MostVisitedURLList& data);

  
  
  void RunUpdate();

  
  
  void CreateIconFiles(const ShellLinkItemList& item_list);

 private:
  friend struct content::BrowserThread::DeleteOnThread<
      content::BrowserThread::UI>;
  friend class base::DeleteHelper<JumpList>;
  ~JumpList();

  
  base::WeakPtrFactory<JumpList> weak_ptr_factory_;

  
  CancelableTaskTracker cancelable_task_tracker_;

  
  Profile* profile_;

  
  scoped_ptr<content::NotificationRegistrar> registrar_;

  
  std::wstring app_id_;

  
  base::FilePath icon_dir_;

  
  
  ShellLinkItemList most_visited_pages_;

  
  
  ShellLinkItemList recently_closed_pages_;

  
  
  typedef std::pair<std::string, scoped_refptr<ShellLinkItem> > URLPair;
  std::list<URLPair> icon_urls_;

  
  
  CancelableTaskTracker::TaskId task_id_;

  
  
  base::Lock list_lock_;

  DISALLOW_COPY_AND_ASSIGN(JumpList);
};

#endif  
