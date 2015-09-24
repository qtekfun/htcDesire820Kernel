// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROME_URL_DATA_MANAGER_BACKEND_H_
#define CHROME_BROWSER_UI_WEBUI_CHROME_URL_DATA_MANAGER_BACKEND_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/task.h"
#include "chrome/browser/ui/webui/chrome_url_data_manager.h"

#include <map>
#include <string>
#include <vector>

class FilePath;
class GURL;
class URLRequestChromeJob;

namespace net {
class URLRequest;
class URLRequestJob;
}

class ChromeURLDataManagerBackend {
 public:
  typedef int RequestID;

  ChromeURLDataManagerBackend();
  ~ChromeURLDataManagerBackend();

  
  static void Register();

  
  void AddDataSource(ChromeURLDataManager::DataSource* source);

  
  void DataAvailable(RequestID request_id, RefCountedMemory* bytes);

  static net::URLRequestJob* Factory(net::URLRequest* request,
                                     const std::string& scheme);

 private:
  friend class URLRequestChromeJob;

  typedef std::map<std::string,
      scoped_refptr<ChromeURLDataManager::DataSource> > DataSourceMap;
  typedef std::map<RequestID, URLRequestChromeJob*> PendingRequestMap;

  
  
  bool StartRequest(const GURL& url, URLRequestChromeJob* job);

  
  void RemoveRequest(URLRequestChromeJob* job);

  
  
  
  bool HasPendingJob(URLRequestChromeJob* job) const;

  
  DataSourceMap data_sources_;

  
  
  
  PendingRequestMap pending_requests_;

  
  RequestID next_request_id_;

  DISALLOW_COPY_AND_ASSIGN(ChromeURLDataManagerBackend);
};

#endif  
