// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_BOOKMARKS_UI_H_
#define CHROME_BROWSER_UI_WEBUI_BOOKMARKS_UI_H_

#include <string>

#include "base/compiler_specific.h"
#include "content/public/browser/url_data_source.h"
#include "content/public/browser/web_ui_controller.h"
#include "ui/base/layout.h"

namespace base {
class RefCountedMemory;
}

class BookmarksUIHTMLSource : public content::URLDataSource {
 public:
  BookmarksUIHTMLSource();

  
  virtual std::string GetSource() const OVERRIDE;
  virtual void StartDataRequest(
      const std::string& path,
      int render_process_id,
      int render_view_id,
      const content::URLDataSource::GotDataCallback& callback) OVERRIDE;
  virtual std::string GetMimeType(const std::string& path) const OVERRIDE;

 private:
  virtual ~BookmarksUIHTMLSource();

  DISALLOW_COPY_AND_ASSIGN(BookmarksUIHTMLSource);
};

class BookmarksUI : public content::WebUIController {
 public:
  explicit BookmarksUI(content::WebUI* web_ui);

  static base::RefCountedMemory* GetFaviconResourceBytes(
      ui::ScaleFactor scale_factor);

 private:
  DISALLOW_COPY_AND_ASSIGN(BookmarksUI);
};

#endif  
