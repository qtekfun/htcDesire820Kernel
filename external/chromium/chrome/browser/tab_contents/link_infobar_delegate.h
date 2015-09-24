// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_LINK_INFOBAR_DELEGATE_H_
#define CHROME_BROWSER_TAB_CONTENTS_LINK_INFOBAR_DELEGATE_H_
#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/string16.h"
#include "chrome/browser/tab_contents/infobar_delegate.h"

class TabContents;

class LinkInfoBarDelegate : public InfoBarDelegate {
 public:
  
  
  virtual string16 GetMessageTextWithOffset(size_t* link_offset) const = 0;

  
  virtual string16 GetLinkText() const = 0;

  
  
  
  
  
  virtual bool LinkClicked(WindowOpenDisposition disposition);

 protected:
  explicit LinkInfoBarDelegate(TabContents* contents);
  virtual ~LinkInfoBarDelegate();

 private:
  
  virtual InfoBar* CreateInfoBar() OVERRIDE;
  virtual LinkInfoBarDelegate* AsLinkInfoBarDelegate() OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(LinkInfoBarDelegate);
};

#endif  
