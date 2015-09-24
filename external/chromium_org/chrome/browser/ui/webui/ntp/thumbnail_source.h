// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NTP_THUMBNAIL_SOURCE_H_
#define CHROME_BROWSER_UI_WEBUI_NTP_THUMBNAIL_SOURCE_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "content/public/browser/url_data_source.h"

class Profile;

namespace base {
class RefCountedMemory;
}

namespace thumbnails {
class ThumbnailService;
}

class ThumbnailSource : public content::URLDataSource {
 public:
  ThumbnailSource(Profile* profile, bool capture_thumbnails);

  
  virtual std::string GetSource() const OVERRIDE;
  virtual void StartDataRequest(
      const std::string& path,
      int render_process_id,
      int render_view_id,
      const content::URLDataSource::GotDataCallback& callback) OVERRIDE;
  virtual std::string GetMimeType(const std::string& path) const OVERRIDE;
  virtual base::MessageLoop* MessageLoopForRequestPath(
      const std::string& path) const OVERRIDE;
  virtual bool ShouldServiceRequest(
      const net::URLRequest* request) const OVERRIDE;

 private:
  virtual ~ThumbnailSource();

  
  
  scoped_refptr<base::RefCountedMemory> default_thumbnail_;

  
  scoped_refptr<thumbnails::ThumbnailService> thumbnail_service_;

  
  Profile* const profile_;

  
  
  
  
  
  
  
  
  const bool capture_thumbnails_;

  DISALLOW_COPY_AND_ASSIGN(ThumbnailSource);
};

#endif  
