// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_EXTENSION_ICON_SOURCE_H_
#define CHROME_BROWSER_UI_WEBUI_EXTENSION_ICON_SOURCE_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "chrome/browser/extensions/image_loading_tracker.h"
#include "chrome/browser/favicon_service.h"
#include "chrome/browser/ui/webui/chrome_url_data_manager.h"
#include "chrome/common/extensions/extension.h"
#include "third_party/skia/include/core/SkBitmap.h"

class ExtensionIconSet;
class Profile;
class RefCountedMemory;

namespace gfx {
class Size;
}

class ExtensionIconSource : public ChromeURLDataManager::DataSource,
                            public ImageLoadingTracker::Observer {
 public:
  explicit ExtensionIconSource(Profile* profile);
  virtual ~ExtensionIconSource();

  
  
  
  static GURL GetIconURL(const Extension* extension,
                         Extension::Icons icon_size,
                         ExtensionIconSet::MatchType match,
                         bool grayscale);

  

  virtual std::string GetMimeType(const std::string&) const;

  virtual void StartDataRequest(const std::string& path,
                                bool is_incognito,
                                int request_id);


 private:
  
  struct ExtensionIconRequest;

  
  SkBitmap* GetDefaultAppImage();

  
  SkBitmap* GetDefaultExtensionImage();

  
  
  
  void FinalizeImage(SkBitmap* image, int request_id);

  
  void LoadDefaultImage(int request_id);

  
  
  void LoadExtensionImage(const ExtensionResource& icon, int request_id);

  
  
  void LoadFaviconImage(int request_id);

  
  void OnFaviconDataAvailable(FaviconService::Handle request_handle,
                              history::FaviconData favicon);

  
  virtual void OnImageLoaded(SkBitmap* image,
                             const ExtensionResource& resource,
                             int id);

  
  
  
  
  
  
  void LoadIconFailed(int request_id);

  
  
  bool ParseData(const std::string& path, int request_id);

  
  void SendDefaultResponse(int request_id);

  
  
  void SetData(int request_id,
               const Extension* extension,
               bool grayscale,
               Extension::Icons size,
               ExtensionIconSet::MatchType match);

  
  ExtensionIconRequest* GetData(int request_id);

  
  void ClearData(int request_id);

  Profile* profile_;

  
  std::map<int, int> tracker_map_;

  
  std::map<int, ExtensionIconRequest*> request_map_;

  scoped_ptr<ImageLoadingTracker> tracker_;

  int next_tracker_id_;

  scoped_ptr<SkBitmap> default_app_data_;

  scoped_ptr<SkBitmap> default_extension_data_;

  CancelableRequestConsumerT<int, 0> cancelable_consumer_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionIconSource);
};

#endif  
