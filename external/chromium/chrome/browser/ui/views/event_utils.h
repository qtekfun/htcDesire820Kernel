// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_EVENT_UTILS_H__
#define CHROME_BROWSER_UI_VIEWS_EVENT_UTILS_H__
#pragma once

#include "webkit/glue/window_open_disposition.h"

namespace views {
class MouseEvent;
}

namespace event_utils {

WindowOpenDisposition DispositionFromEventFlags(int event_flags);

bool IsPossibleDispositionEvent(const views::MouseEvent& event);

}

#endif  
