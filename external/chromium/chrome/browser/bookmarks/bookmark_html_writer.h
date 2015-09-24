// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_HTML_WRITER_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_HTML_WRITER_H_
#pragma once

#include <list>
#include <map>
#include <string>

#include "base/memory/ref_counted.h"
#include "chrome/browser/history/history.h"
#include "content/common/notification_registrar.h"
#include "net/base/file_stream.h"

class BookmarkNode;
class FilePath;
class GURL;
class Profile;

class BookmarksExportObserver {
 public:
  
  virtual void OnExportFinished() = 0;

 protected:
  virtual ~BookmarksExportObserver() {}
};

class BookmarkFaviconFetcher: public NotificationObserver {
 public:
  
  typedef std::map<std::string, scoped_refptr<RefCountedMemory> > URLFaviconMap;

  BookmarkFaviconFetcher(Profile* profile,
                         const FilePath& path,
                         BookmarksExportObserver* observer);
  ~BookmarkFaviconFetcher();

  
  void ExportBookmarks();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  
  void ExtractUrls(const BookmarkNode* node);

  
  void ExecuteWriter();

  
  
  
  bool FetchNextFavicon();

  
  
  void OnFaviconDataAvailable(FaviconService::Handle handle,
                              history::FaviconData favicon);

  
  Profile* profile_;

  
  
  std::list<std::string> bookmark_urls_;

  
  CancelableRequestConsumer favicon_consumer_;

  
  scoped_ptr<URLFaviconMap> favicons_map_;

  
  FilePath path_;

  BookmarksExportObserver* observer_;

  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkFaviconFetcher);
};

namespace bookmark_html_writer {

// Firefox and IE. The results are written to the file at |path|.  The file
void WriteBookmarks(Profile* profile,
                    const FilePath& path,
                    BookmarksExportObserver* observer);

}  

#endif  
