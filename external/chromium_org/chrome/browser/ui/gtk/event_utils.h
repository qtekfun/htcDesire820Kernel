// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_EVENT_UTILS_H_
#define CHROME_BROWSER_UI_GTK_EVENT_UTILS_H_

#include <gtk/gtk.h>

#include "ui/base/window_open_disposition.h"

namespace event_utils {

int EventFlagsFromGdkState(guint state);

WindowOpenDisposition DispositionFromGdkState(guint state);

WindowOpenDisposition DispositionForCurrentButtonPressEvent();

}  

#endif  
