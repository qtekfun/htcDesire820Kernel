// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_THUMBNAIL_SOURCE_H_
#define CHROME_BROWSER_UI_WEBUI_THUMBNAIL_SOURCE_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/ref_counted_memory.h"
#include "chrome/browser/ui/webui/chrome_url_data_manager.h"

class Profile;

namespace history {
class TopSites;
}

class ThumbnailSource : public ChromeURLDataManager::DataSource {
 public:
  explicit ThumbnailSource(Profile* profile);

  
  
  virtual void StartDataRequest(const std::string& path,
                                bool is_incognito,
                                int request_id);

  virtual std::string GetMimeType(const std::string& path) const;

  virtual MessageLoop* MessageLoopForRequestPath(const std::string& path) const;

 private:
  virtual ~ThumbnailSource();

  
  void SendDefaultThumbnail(int request_id);

  
  
  scoped_refptr<RefCountedMemory> default_thumbnail_;

  
  scoped_refptr<history::TopSites> top_sites_;

  DISALLOW_COPY_AND_ASSIGN(ThumbnailSource);
};

#endif  
