// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_HTML_WRITER_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_HTML_WRITER_H_

#include <list>
#include <map>
#include <string>

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/ref_counted_memory.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class BookmarkNode;
class Profile;

namespace chrome {
struct FaviconBitmapResult;
}

class BookmarksExportObserver {
 public:
  
  virtual void OnExportFinished() = 0;

 protected:
  virtual ~BookmarksExportObserver() {}
};

class BookmarkFaviconFetcher: public content::NotificationObserver {
 public:
  
  typedef std::map<std::string, scoped_refptr<base::RefCountedMemory> >
      URLFaviconMap;

  BookmarkFaviconFetcher(Profile* profile,
                         const base::FilePath& path,
                         BookmarksExportObserver* observer);
  virtual ~BookmarkFaviconFetcher();

  
  void ExportBookmarks();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  
  void ExtractUrls(const BookmarkNode* node);

  
  void ExecuteWriter();

  
  
  
  bool FetchNextFavicon();

  
  
  void OnFaviconDataAvailable(const chrome::FaviconBitmapResult& bitmap_result);

  
  Profile* profile_;

  
  
  std::list<std::string> bookmark_urls_;

  
  CancelableTaskTracker cancelable_task_tracker_;

  
  scoped_ptr<URLFaviconMap> favicons_map_;

  
  base::FilePath path_;

  BookmarksExportObserver* observer_;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkFaviconFetcher);
};

namespace bookmark_html_writer {

// Firefox and IE. The results are written to the file at |path|.  The file
void WriteBookmarks(Profile* profile,
                    const base::FilePath& path,
                    BookmarksExportObserver* observer);

}  

#endif  
