// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_COMPONENT_UPDATER_CRX_DOWNLOADER_H_
#define CHROME_BROWSER_COMPONENT_UPDATER_CRX_DOWNLOADER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/sequenced_task_runner.h"
#include "url/gurl.h"

namespace net {
class URLRequestContextGetter;
}

namespace component_updater {

class CrxDownloader {
 public:
  struct DownloadMetrics {
    enum Downloader {
      kNone = 0,
      kUrlFetcher,
      kBits
    };

    DownloadMetrics();

    GURL url;

    Downloader downloader;

    int error;

    int64 bytes_downloaded;   
    int64 bytes_total;

    uint64 download_time_ms;
  };

  
  struct Result {
    Result();

    
    int error;

    
    base::FilePath response;
  };

  
  
  
  
  
  typedef base::Callback<void (const Result& result)> DownloadCallback;

  
  
  
  static CrxDownloader* Create(
      bool is_background_download,
      net::URLRequestContextGetter* context_getter,
      scoped_refptr<base::SequencedTaskRunner> task_runner,
      const DownloadCallback& download_callback);
  virtual ~CrxDownloader();

  
  
  
  
  void StartDownloadFromUrl(const GURL& url);
  void StartDownload(const std::vector<GURL>& urls);

  const std::vector<DownloadMetrics> download_metrics() const;

 protected:
  CrxDownloader(scoped_ptr<CrxDownloader> successor,
                const DownloadCallback& download_callback);

  
  
  
  
  
  
  
  
  void OnDownloadComplete(bool is_handled,
                          const Result& result,
                          const DownloadMetrics& download_metrics);

  
  GURL url() const;

 private:
  virtual void DoStartDownload(const GURL& url) = 0;

  std::vector<GURL> urls_;
  scoped_ptr<CrxDownloader> successor_;
  DownloadCallback download_callback_;

  std::vector<GURL>::iterator current_url_;

  std::vector<DownloadMetrics> download_metrics_;

  DISALLOW_COPY_AND_ASSIGN(CrxDownloader);
};

}  

#endif  

