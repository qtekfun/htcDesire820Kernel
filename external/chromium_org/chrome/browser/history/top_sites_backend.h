// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_TOP_SITES_BACKEND_H_
#define CHROME_BROWSER_HISTORY_TOP_SITES_BACKEND_H_

#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/history/history_types.h"

class CancelableTaskTracker;

namespace base {
class FilePath;
}

namespace history {

class TopSitesDatabase;

class TopSitesBackend : public base::RefCountedThreadSafe<TopSitesBackend> {
 public:
  
  
  typedef base::Callback<void(const scoped_refptr<MostVisitedThumbnails>&)>
      GetMostVisitedThumbnailsCallback;

  TopSitesBackend();

  void Init(const base::FilePath& path);

  
  void Shutdown();

  
  void GetMostVisitedThumbnails(
      const GetMostVisitedThumbnailsCallback& callback,
      CancelableTaskTracker* tracker);

  
  void UpdateTopSites(const TopSitesDelta& delta);

  
  void SetPageThumbnail(const MostVisitedURL& url,
                        int url_rank,
                        const Images& thumbnail);

  
  void ResetDatabase();

  
  
  
  void DoEmptyRequest(const base::Closure& reply,
                      CancelableTaskTracker* tracker);

 private:
  friend class base::RefCountedThreadSafe<TopSitesBackend>;

  virtual ~TopSitesBackend();

  
  void InitDBOnDBThread(const base::FilePath& path);

  
  void ShutdownDBOnDBThread();

  
  void GetMostVisitedThumbnailsOnDBThread(
      scoped_refptr<MostVisitedThumbnails> thumbnails);

  
  void UpdateTopSitesOnDBThread(const TopSitesDelta& delta);

  
  void SetPageThumbnailOnDBThread(const MostVisitedURL& url,
                                  int url_rank,
                                  const Images& thumbnail);

  
  void ResetDatabaseOnDBThread(const base::FilePath& file_path);

  base::FilePath db_path_;

  scoped_ptr<TopSitesDatabase> db_;

  DISALLOW_COPY_AND_ASSIGN(TopSitesBackend);
};

}  

#endif  
