// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_REQUEST_INFOBAR_DELEGATE_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_REQUEST_INFOBAR_DELEGATE_H_
#pragma once

#include "base/basictypes.h"
#include "chrome/browser/download/download_request_limiter.h"
#include "chrome/browser/tab_contents/confirm_infobar_delegate.h"

class TabContents;

class DownloadRequestInfoBarDelegate : public ConfirmInfoBarDelegate {
 public:
  DownloadRequestInfoBarDelegate(
      TabContents* tab,
      DownloadRequestLimiter::TabDownloadState* host);

  void set_host(DownloadRequestLimiter::TabDownloadState* host) {
    host_ = host;
  }

 private:
  virtual ~DownloadRequestInfoBarDelegate();

  
  virtual void InfoBarClosed();
  virtual SkBitmap* GetIcon() const;
  virtual string16 GetMessageText() const;
  virtual string16 GetButtonLabel(InfoBarButton button) const;
  virtual bool Accept();

  DownloadRequestLimiter::TabDownloadState* host_;

  DISALLOW_COPY_AND_ASSIGN(DownloadRequestInfoBarDelegate);
};

#endif  
