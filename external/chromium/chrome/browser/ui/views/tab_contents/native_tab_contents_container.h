// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TAB_CONTENTS_NATIVE_TAB_CONTENTS_CONTAINER_H_
#define CHROME_BROWSER_UI_VIEWS_TAB_CONTENTS_NATIVE_TAB_CONTENTS_CONTAINER_H_
#pragma once

class RenderViewHost;
class TabContents;
class TabContentsContainer;
namespace views {
class View;
}

class NativeTabContentsContainer {
 public:
  
  static NativeTabContentsContainer* CreateNativeContainer(
      TabContentsContainer* container);

  
  virtual void AttachContents(TabContents* contents) = 0;

  
  virtual void DetachContents(TabContents* contents) = 0;

  
  
  virtual void SetFastResize(bool fast_resize) = 0;

  
  
  virtual void RenderViewHostChanged(RenderViewHost* old_host,
                                     RenderViewHost* new_host) = 0;

  
  virtual void TabContentsFocused(TabContents* tab_contents) = 0;

  
  virtual views::View* GetView() = 0;
 protected:
  virtual ~NativeTabContentsContainer() {}
};

#endif  
