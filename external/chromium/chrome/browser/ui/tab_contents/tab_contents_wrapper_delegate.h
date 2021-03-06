// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TAB_CONTENTS_TAB_CONTENTS_WRAPPER_DELEGATE_H_
#define CHROME_BROWSER_UI_TAB_CONTENTS_TAB_CONTENTS_WRAPPER_DELEGATE_H_
#pragma once

#include "base/basictypes.h"

class TabContentsWrapper;
struct WebApplicationInfo;

class TabContentsWrapperDelegate {
 public:
  
  virtual void URLStarredChanged(TabContentsWrapper* source, bool starred) = 0;

  
  virtual void OnDidGetApplicationInfo(TabContentsWrapper* source,
                                       int32 page_id);

  
  virtual void OnInstallApplication(TabContentsWrapper* source,
                                    const WebApplicationInfo& app_info);

 protected:
  virtual ~TabContentsWrapperDelegate();
};

#endif  
