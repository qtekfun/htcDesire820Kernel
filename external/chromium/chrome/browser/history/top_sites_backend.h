// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_TOP_SITES_BACKEND_H_
#define CHROME_BROWSER_HISTORY_TOP_SITES_BACKEND_H_
#pragma once

#include "base/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/history/history_types.h"
#include "content/browser/cancelable_request.h"

class FilePath;

namespace history {

class TopSitesDatabase;

class TopSitesBackend
    : public base::RefCountedThreadSafe<TopSitesBackend>,
      public CancelableRequestProvider {
 public:
  TopSitesBackend();

  void Init(const FilePath& path);

  
  void Shutdown();

  
  
  typedef Callback3<Handle, scoped_refptr<MostVisitedThumbnails>, bool >::Type
      GetMostVisitedThumbnailsCallback;
  typedef CancelableRequest1<TopSitesBackend::GetMostVisitedThumbnailsCallback,
                             scoped_refptr<MostVisitedThumbnails> >
      GetMostVisitedThumbnailsRequest;

  
  Handle GetMostVisitedThumbnails(CancelableRequestConsumerBase* consumer,
                                  GetMostVisitedThumbnailsCallback* callback);

  
  void UpdateTopSites(const TopSitesDelta& delta);

  
  void SetPageThumbnail(const MostVisitedURL& url,
                        int url_rank,
                        const Images& thumbnail);

  
  void ResetDatabase();

  typedef Callback1<Handle>::Type EmptyRequestCallback;
  typedef CancelableRequest<TopSitesBackend::EmptyRequestCallback>
      EmptyRequestRequest;

  
  
  
  Handle DoEmptyRequest(CancelableRequestConsumerBase* consumer,
                        EmptyRequestCallback* callback);

 private:
  friend class base::RefCountedThreadSafe<TopSitesBackend>;

  ~TopSitesBackend();

  
  void InitDBOnDBThread(const FilePath& path);

  
  void ShutdownDBOnDBThread();

  
  void GetMostVisitedThumbnailsOnDBThread(
      scoped_refptr<GetMostVisitedThumbnailsRequest> request);

  
  void UpdateTopSitesOnDBThread(const TopSitesDelta& delta);

  
  void SetPageThumbnailOnDBThread(const MostVisitedURL& url,
                                  int url_rank,
                                  const Images& thumbnail);

  
  void ResetDatabaseOnDBThread(const FilePath& file_path);

  
  void DoEmptyRequestOnDBThread(scoped_refptr<EmptyRequestRequest> request);

  FilePath db_path_;

  scoped_ptr<TopSitesDatabase> db_;

  DISALLOW_COPY_AND_ASSIGN(TopSitesBackend);
};

}  

#endif  
