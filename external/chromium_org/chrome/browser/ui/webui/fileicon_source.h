// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_FILEICON_SOURCE_H_
#define CHROME_BROWSER_UI_WEBUI_FILEICON_SOURCE_H_

#include <string>

#include "base/files/file_path.h"
#include "chrome/browser/icon_manager.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "content/public/browser/url_data_source.h"
#include "ui/base/layout.h"

namespace gfx {
class Image;
}

class FileIconSource : public content::URLDataSource {
 public:
  explicit FileIconSource();

  
  virtual std::string GetSource() const OVERRIDE;
  virtual void StartDataRequest(
      const std::string& path,
      int render_process_id,
      int render_view_id,
      const content::URLDataSource::GotDataCallback& callback) OVERRIDE;
  virtual std::string GetMimeType(const std::string&) const OVERRIDE;

 protected:
  virtual ~FileIconSource();

  
  
  
  virtual void FetchFileIcon(
      const base::FilePath& path,
      ui::ScaleFactor scale_factor,
      IconLoader::IconSize icon_size,
      const content::URLDataSource::GotDataCallback& callback);

 private:
  
  struct IconRequestDetails {
    IconRequestDetails();
    ~IconRequestDetails();

    
    content::URLDataSource::GotDataCallback callback;

    
    ui::ScaleFactor scale_factor;
  };

  
  void OnFileIconDataAvailable(const IconRequestDetails& details,
                               gfx::Image* icon);

  
  CancelableTaskTracker cancelable_task_tracker_;

  DISALLOW_COPY_AND_ASSIGN(FileIconSource);
};
#endif  
