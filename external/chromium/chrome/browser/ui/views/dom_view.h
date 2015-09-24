// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_VIEWS_DOM_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_DOM_VIEW_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "googleurl/src/gurl.h"
#include "views/controls/native/native_view_host.h"
#include "views/events/event.h"

class Profile;
class SiteInstance;
class TabContents;

class DOMView : public views::NativeViewHost {
 public:
  DOMView();
  virtual ~DOMView();

  
  
  
  
  
  bool Init(Profile* profile, SiteInstance* instance);

  
  void LoadURL(const GURL& url);

  
  TabContents* tab_contents() const { return tab_contents_.get(); }

 protected:
  
  virtual bool SkipDefaultKeyEventProcessing(const views::KeyEvent& e) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void ViewHierarchyChanged(bool is_add, views::View* parent,
                                    views::View* child) OVERRIDE;

  
  
  
  void AttachTabContents();

  
  
  virtual TabContents* CreateTabContents(Profile* profile,
                                         SiteInstance* instance);

  scoped_ptr<TabContents> tab_contents_;

 private:
  bool initialized_;

  DISALLOW_COPY_AND_ASSIGN(DOMView);
};

#endif  
