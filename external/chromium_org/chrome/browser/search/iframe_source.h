// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_IFRAME_SOURCE_H_
#define CHROME_BROWSER_SEARCH_IFRAME_SOURCE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "content/public/browser/url_data_source.h"

class IframeSource : public content::URLDataSource {
 public:
  IframeSource();
  virtual ~IframeSource();

 protected:
  
  virtual std::string GetMimeType(
      const std::string& path_and_query) const OVERRIDE;
  virtual bool ShouldDenyXFrameOptions() const OVERRIDE;
  virtual bool ShouldServiceRequest(
      const net::URLRequest* request) const OVERRIDE;

  
  virtual bool ServesPath(const std::string& path) const = 0;

  
  void SendResource(
      int resource_id,
      const content::URLDataSource::GotDataCallback& callback);

  
  void SendJSWithOrigin(
      int resource_id,
      int render_process_id,
      int render_view_id,
      const content::URLDataSource::GotDataCallback& callback);

  
  
  
  
  virtual bool GetOrigin(
      int process_id,
      int render_view_id,
      std::string* origin) const;

  DISALLOW_COPY_AND_ASSIGN(IframeSource);
};

#endif  
