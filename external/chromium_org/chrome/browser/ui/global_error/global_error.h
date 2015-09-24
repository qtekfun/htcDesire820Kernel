// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GLOBAL_ERROR_GLOBAL_ERROR_H_
#define CHROME_BROWSER_UI_GLOBAL_ERROR_GLOBAL_ERROR_H_

#include <vector>

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"

class Browser;
class GlobalErrorBubbleViewBase;

namespace gfx {
class Image;
}

class GlobalError {
 public:
  enum Severity {
    SEVERITY_LOW,
    SEVERITY_MEDIUM,
    SEVERITY_HIGH,
  };

  GlobalError();
  virtual ~GlobalError();

  
  
  
  virtual Severity GetSeverity();

  
  virtual bool HasMenuItem() = 0;
  
  virtual int MenuItemCommandID() = 0;
  
  virtual base::string16 MenuItemLabel() = 0;
  
  int MenuItemIconResourceID();
  
  virtual void ExecuteMenuItem(Browser* browser) = 0;

  
  virtual bool HasBubbleView() = 0;
  
  virtual bool HasShownBubbleView() = 0;
  
  virtual void ShowBubbleView(Browser* browser) = 0;
  
  virtual GlobalErrorBubbleViewBase* GetBubbleView() = 0;
};

class GlobalErrorWithStandardBubble
    : public GlobalError,
      public base::SupportsWeakPtr<GlobalErrorWithStandardBubble> {
 public:
  GlobalErrorWithStandardBubble();
  virtual ~GlobalErrorWithStandardBubble();

  
  virtual gfx::Image GetBubbleViewIcon();
  
  virtual base::string16 GetBubbleViewTitle() = 0;
  
  
  
  virtual std::vector<base::string16> GetBubbleViewMessages() = 0;
  
  virtual base::string16 GetBubbleViewAcceptButtonLabel() = 0;
  
  
  virtual base::string16 GetBubbleViewCancelButtonLabel() = 0;
  
  
  void BubbleViewDidClose(Browser* browser);
  
  
  virtual void OnBubbleViewDidClose(Browser* browser) = 0;
  
  
  virtual void BubbleViewAcceptButtonPressed(Browser* browser) = 0;
  
  
  virtual void BubbleViewCancelButtonPressed(Browser* browser) = 0;

  
  virtual bool HasBubbleView() OVERRIDE;
  virtual bool HasShownBubbleView() OVERRIDE;
  virtual void ShowBubbleView(Browser* browser) OVERRIDE;
  virtual GlobalErrorBubbleViewBase* GetBubbleView() OVERRIDE;

 private:
  bool has_shown_bubble_view_;
  GlobalErrorBubbleViewBase* bubble_view_;

  DISALLOW_COPY_AND_ASSIGN(GlobalErrorWithStandardBubble);
};

#endif  
