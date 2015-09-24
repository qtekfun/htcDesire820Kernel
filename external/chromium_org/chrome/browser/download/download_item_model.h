// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_ITEM_MODEL_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_ITEM_MODEL_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/strings/string16.h"

class SavePackage;

namespace content {
class DownloadItem;
}

namespace gfx {
class FontList;
}

class DownloadItemModel {
 public:
  
  
  explicit DownloadItemModel(content::DownloadItem* download);
  ~DownloadItemModel();

  
  
  base::string16 GetInterruptReasonText() const;

  
  base::string16 GetStatusText() const;

  
  
  base::string16 GetTabProgressStatusText() const;

  
  
  
  
  
  
  
  
  
  base::string16 GetTooltipText(const gfx::FontList& font_list, int max_width) const;

  
  
  
  
  base::string16 GetWarningText(const gfx::FontList& font_list, int base_width) const;

  
  
  base::string16 GetWarningConfirmButtonText() const;

  
  int64 GetCompletedBytes() const;

  
  
  int64 GetTotalBytes() const;

  
  int PercentComplete() const;

  
  bool IsDangerous() const;

  
  bool MightBeMalicious() const;

  
  
  bool IsMalicious() const;

  
  bool ShouldAllowDownloadFeedback() const;

  
  
  
  
  
  
  
  
  
  
  bool ShouldRemoveFromShelfWhenComplete() const;

  
  
  
  
  bool ShouldShowDownloadStartedAnimation() const;

  
  bool ShouldShowInShelf() const;

  
  
  
  void SetShouldShowInShelf(bool should_show);

  
  
  
  
  
  bool ShouldNotifyUI() const;

  
  void SetShouldNotifyUI(bool should_notify);

  
  
  bool ShouldPreferOpeningInBrowser() const;

  
  void SetShouldPreferOpeningInBrowser(bool preference);

  
  
  
  void OpenUsingPlatformHandler();

  content::DownloadItem* download() { return download_; }

 private:
  
  
  
  
  
  base::string16 GetProgressSizesString() const;

  
  base::string16 GetInProgressStatusString() const;

  
  
  
  content::DownloadItem* download_;

  DISALLOW_COPY_AND_ASSIGN(DownloadItemModel);
};

#endif  
