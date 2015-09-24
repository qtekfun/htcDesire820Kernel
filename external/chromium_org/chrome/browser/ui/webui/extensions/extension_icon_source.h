// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_EXTENSIONS_EXTENSION_ICON_SOURCE_H_
#define CHROME_BROWSER_UI_WEBUI_EXTENSIONS_EXTENSION_ICON_SOURCE_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/favicon/favicon_service.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "chrome/common/extensions/extension_icon_set.h"
#include "content/public/browser/url_data_source.h"
#include "extensions/common/extension_resource.h"
#include "third_party/skia/include/core/SkBitmap.h"

class ExtensionIconSet;
class Profile;

namespace extensions {
class Extension;

class ExtensionIconSource : public content::URLDataSource,
                            public base::SupportsWeakPtr<ExtensionIconSource> {
 public:
  explicit ExtensionIconSource(Profile* profile);

  
  
  
  
  
  static GURL GetIconURL(const Extension* extension,
                         int icon_size,
                         ExtensionIconSet::MatchType match,
                         bool grayscale,
                         bool* exists);

  
  
  static SkBitmap* LoadImageByResourceId(int resource_id);

  
  virtual std::string GetSource() const OVERRIDE;
  virtual std::string GetMimeType(const std::string&) const OVERRIDE;
  virtual void StartDataRequest(
      const std::string& path,
      int render_process_id,
      int render_view_id,
      const content::URLDataSource::GotDataCallback& callback) OVERRIDE;

 private:
  
  struct ExtensionIconRequest;

  virtual ~ExtensionIconSource();

  
  const SkBitmap* GetDefaultAppImage();

  
  const SkBitmap* GetDefaultExtensionImage();

  
  
  
  void FinalizeImage(const SkBitmap* image, int request_id);

  
  void LoadDefaultImage(int request_id);

  
  
  void LoadExtensionImage(const ExtensionResource& icon,
                          int request_id);

  
  
  void LoadFaviconImage(int request_id);

  
  void OnFaviconDataAvailable(
      int request_id,
      const chrome::FaviconBitmapResult& bitmap_result);

  
  void OnImageLoaded(int request_id, const gfx::Image& image);

  
  
  
  
  
  
  void LoadIconFailed(int request_id);

  
  
  bool ParseData(const std::string& path,
                 int request_id,
                 const content::URLDataSource::GotDataCallback& callback);

  
  
  void SetData(int request_id,
               const content::URLDataSource::GotDataCallback& callback,
               const Extension* extension,
               bool grayscale,
               int size,
               ExtensionIconSet::MatchType match);

  
  ExtensionIconRequest* GetData(int request_id);

  
  void ClearData(int request_id);

  Profile* profile_;

  
  std::map<int, int> tracker_map_;

  
  std::map<int, ExtensionIconRequest*> request_map_;

  scoped_ptr<SkBitmap> default_app_data_;

  scoped_ptr<SkBitmap> default_extension_data_;

  CancelableTaskTracker cancelable_task_tracker_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionIconSource);
};

}  

#endif  
