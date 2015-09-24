// Copyright 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_WINDOW_OPEN_DISPOSITION_H_
#define UI_BASE_WINDOW_OPEN_DISPOSITION_H_

#include "ui/base/ui_export.h"

enum WindowOpenDisposition {

#define WINDOW_OPEN_DISPOSITION(label, value) label = value,
#include "window_open_disposition_list.h"
#undef WINDOW_OPEN_DISPOSITION

};

namespace ui {

UI_EXPORT WindowOpenDisposition DispositionFromClick(bool middle_button,
                                                     bool alt_key,
                                                     bool ctrl_key,
                                                     bool meta_key,
                                                     bool shift_key);

UI_EXPORT WindowOpenDisposition DispositionFromEventFlags(int event_flags);

}  

#endif  
