// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WEBUI_URL_DATA_MANAGER_H_
#define CONTENT_BROWSER_WEBUI_URL_DATA_MANAGER_H_

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/supports_user_data.h"
#include "content/common/content_export.h"

namespace content {
class BrowserContext;
class URLDataSource;
class URLDataSourceImpl;
class WebUIDataSource;

class CONTENT_EXPORT URLDataManager : public base::SupportsUserData::Data {
 public:
  explicit URLDataManager(BrowserContext* browser_context);
  virtual ~URLDataManager();

  
  
  
  
  
  
  
  
  
  
  void AddDataSource(URLDataSourceImpl* source);

  
  
  static void DeleteDataSources();

  
  
  
  static void AddDataSource(BrowserContext* browser_context,
                            URLDataSource* source);

  
  static void AddWebUIDataSource(BrowserContext* browser_context,
                                 WebUIDataSource* source);

 private:
  friend class URLDataSourceImpl;
  friend struct DeleteURLDataSource;
  typedef std::vector<const URLDataSourceImpl*> URLDataSources;

  
  
  
  static void DeleteDataSource(const URLDataSourceImpl* data_source);

  
  
  static bool IsScheduledForDeletion(const URLDataSourceImpl* data_source);

  BrowserContext* browser_context_;

  
  
  static URLDataSources* data_sources_;

  DISALLOW_COPY_AND_ASSIGN(URLDataManager);
};

}  

#endif  
