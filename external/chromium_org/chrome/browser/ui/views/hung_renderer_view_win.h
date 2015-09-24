// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_HUNG_RENDERER_VIEW_WIN_H_
#define CHROME_BROWSER_UI_VIEWS_HUNG_RENDERER_VIEW_WIN_H_

#include "base/basictypes.h"
#include "content/public/browser/web_contents.h"

class HungRendererDialogMetro {
 public:
  
  
  static HungRendererDialogMetro* Create();
  static HungRendererDialogMetro* GetInstance();

  void Show(content::WebContents* contents);
  void Hide(content::WebContents* contents);

 private:
  HungRendererDialogMetro();
  ~HungRendererDialogMetro();

  
  static void OnMetroKillProcess();
  static void OnMetroWait();

  
  
  void ResetMetroState();

  content::WebContents* contents_;

  
  
  
  bool metro_dialog_displayed_;

  
  static HungRendererDialogMetro* g_instance_;

  DISALLOW_COPY_AND_ASSIGN(HungRendererDialogMetro);
};

#endif  

