// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_PROCESS_SINGLETON_DIALOG_LINUX_H_
#define CHROME_BROWSER_UI_PROCESS_SINGLETON_DIALOG_LINUX_H_

#include "base/strings/string16.h"


bool ShowProcessSingletonDialog(const base::string16& message,
                                const base::string16& relaunch_text);

#endif  
