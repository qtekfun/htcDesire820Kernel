// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_BROWSER_GL_VIEW_RENDERER_MANAGER_H_
#define ANDROID_WEBVIEW_BROWSER_GL_VIEW_RENDERER_MANAGER_H_

#include <list>

#include "base/basictypes.h"

namespace android_webview {

class BrowserViewRenderer;

class GLViewRendererManager {
  typedef std::list<BrowserViewRenderer*> ListType;
 public:
  typedef ListType::iterator Key;

  GLViewRendererManager();
  ~GLViewRendererManager();

  
  
  
  Key DidDrawGL(Key key, BrowserViewRenderer* view);

  void NoLongerExpectsDrawGL(Key key);

  BrowserViewRenderer* GetMostRecentlyDrawn() const;

  Key NullKey();

 private:
  ListType mru_list_;

  DISALLOW_COPY_AND_ASSIGN(GLViewRendererManager);
};

}  

#endif  
