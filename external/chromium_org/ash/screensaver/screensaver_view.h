// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SCREENSAVER_SCREENSAVER_VIEW_H_
#define ASH_SCREENSAVER_SCREENSAVER_VIEW_H_

#include "ash/ash_export.h"
#include "base/callback.h"
#include "content/public/browser/web_contents_observer.h"
#include "ui/views/widget/widget_delegate.h"
#include "url/gurl.h"

namespace content {
class BrowserContent;
}

namespace views {
class WebView;
}

namespace ash {

namespace test {
class ScreensaverViewTest;
}

ASH_EXPORT void ShowScreensaver(const GURL& url);
ASH_EXPORT void CloseScreensaver();
ASH_EXPORT bool IsScreensaverShown();

typedef
    base::Callback<views::WebView*(content::BrowserContext*)> WebViewFactory;

namespace internal {

class ScreensaverView : public views::WidgetDelegateView,
                        public content::WebContentsObserver {
 public:
  static void ShowScreensaver(const GURL& url);
  static void CloseScreensaver();

  static bool IsScreensaverShown();

 private:
  friend class test::ScreensaverViewTest;

  explicit ScreensaverView(const GURL& url);
  virtual ~ScreensaverView();

  
  virtual views::View* GetContentsView() OVERRIDE;

  
  virtual void RenderProcessGone(base::TerminationStatus status) OVERRIDE;

  void Show();
  void Close();

  
  void AddChildWebContents();
  
  
  void LoadScreensaver();
  
  void ShowWindow();

  
  static ASH_EXPORT ScreensaverView* GetInstance();
  ASH_EXPORT bool IsScreensaverShowingURL(const GURL& url);

  
  GURL url_;

  
  
  int termination_count_;

  
  views::WebView* screensaver_webview_;

  
  views::Widget* container_window_;

  DISALLOW_COPY_AND_ASSIGN(ScreensaverView);
};

}  
}  

#endif  
