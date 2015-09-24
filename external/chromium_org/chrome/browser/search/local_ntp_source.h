// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_LOCAL_NTP_SOURCE_H_
#define CHROME_BROWSER_SEARCH_LOCAL_NTP_SOURCE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "content/public/browser/url_data_source.h"

class Profile;

class LocalNtpSource : public content::URLDataSource {
 public:
  explicit LocalNtpSource(Profile* profile);

 private:
  virtual ~LocalNtpSource();

  
  virtual std::string GetSource() const OVERRIDE;
  virtual void StartDataRequest(
      const std::string& path,
      int render_process_id,
      int render_view_id,
      const content::URLDataSource::GotDataCallback& callback) OVERRIDE;
  virtual std::string GetMimeType(const std::string& path) const OVERRIDE;
  virtual bool ShouldServiceRequest(
      const net::URLRequest* request) const OVERRIDE;
  virtual std::string GetContentSecurityPolicyFrameSrc() const OVERRIDE;

  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(LocalNtpSource);
};

#endif  
