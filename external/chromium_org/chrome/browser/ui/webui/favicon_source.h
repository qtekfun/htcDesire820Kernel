// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_FAVICON_SOURCE_H_
#define CHROME_BROWSER_UI_WEBUI_FAVICON_SOURCE_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/favicon/favicon_service.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "content/public/browser/url_data_source.h"
#include "ui/gfx/favicon_size.h"

class Profile;

class FaviconSource : public content::URLDataSource {
 public:
  
  enum IconType {
    FAVICON,
    
    
    ANY
  };

  
  FaviconSource(Profile* profile, IconType type);

  virtual ~FaviconSource();

  
  virtual std::string GetSource() const OVERRIDE;
  virtual void StartDataRequest(
      const std::string& path,
      int render_process_id,
      int render_view_id,
      const content::URLDataSource::GotDataCallback& callback) OVERRIDE;
  virtual std::string GetMimeType(const std::string&) const OVERRIDE;
  virtual bool ShouldReplaceExistingSource() const OVERRIDE;
  virtual bool ShouldServiceRequest(
      const net::URLRequest* request) const OVERRIDE;

 protected:
  struct IconRequest {
    IconRequest();
    IconRequest(const content::URLDataSource::GotDataCallback& cb,
                const GURL& path,
                int size,
                ui::ScaleFactor scale);
    ~IconRequest();

    content::URLDataSource::GotDataCallback callback;
    GURL request_path;
    int size_in_dip;
    ui::ScaleFactor scale_factor;
  };

  
  
  
  
  virtual bool HandleMissingResource(const IconRequest& request);

  Profile* profile_;

 private:
  FRIEND_TEST_ALL_PREFIXES(FaviconSourceTest, InstantParsing);
  FRIEND_TEST_ALL_PREFIXES(FaviconSourceTest, Parsing);

  
  enum IconSize {
    SIZE_16,
    SIZE_32,
    SIZE_64,
    NUM_SIZES
  };

  
  void OnFaviconDataAvailable(
      const IconRequest& request,
      const chrome::FaviconBitmapResult& bitmap_result);

  
  void SendDefaultResponse(
      const content::URLDataSource::GotDataCallback& callback);

  
  void SendDefaultResponse(const IconRequest& request);

  CancelableTaskTracker cancelable_task_tracker_;

  
  
  scoped_refptr<base::RefCountedMemory> default_favicons_[NUM_SIZES];

  
  int icon_types_;

  DISALLOW_COPY_AND_ASSIGN(FaviconSource);
};

#endif  
