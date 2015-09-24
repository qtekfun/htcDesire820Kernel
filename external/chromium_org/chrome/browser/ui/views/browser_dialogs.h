// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_BROWSER_DIALOGS_H_
#define CHROME_BROWSER_UI_VIEWS_BROWSER_DIALOGS_H_

#include "base/callback_forward.h"
#include "ui/gfx/native_widget_types.h"


class BrowserView;
class EditSearchEngineControllerDelegate;
class FindBar;
class Profile;
class TemplateURL;

namespace chrome {

FindBar* CreateFindBar(BrowserView* browser_view);

void EditSearchEngine(gfx::NativeWindow parent,
                      TemplateURL* template_url,
                      EditSearchEngineControllerDelegate* delegate,
                      Profile* profile);

}  

#endif  
