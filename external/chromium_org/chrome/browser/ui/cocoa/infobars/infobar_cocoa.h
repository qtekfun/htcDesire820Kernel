// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_INFOBARS_INFOBAR_COCOA_H_
#define CHROME_BROWSER_UI_COCOA_INFOBARS_INFOBAR_COCOA_H_

#include "base/mac/scoped_nsobject.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/infobars/infobar.h"

@class InfoBarController;

class InfoBarCocoa : public InfoBar {
 public:
  explicit InfoBarCocoa(scoped_ptr<InfoBarDelegate> delegate);

  virtual ~InfoBarCocoa();

  InfoBarController* controller() const { return controller_; }

  void set_controller(InfoBarController* controller) {
    controller_.reset([controller retain]);
  }

  
  InfoBarService* OwnerCocoa();

  base::WeakPtr<InfoBarCocoa> GetWeakPtr();

 private:
  
  base::scoped_nsobject<InfoBarController> controller_;

  
  base::WeakPtrFactory<InfoBarCocoa> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(InfoBarCocoa);
};

#endif  
