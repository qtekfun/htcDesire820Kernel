// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_SHELL_DIALOGS_SELECT_FILE_DIALOG_WIN_H_
#define UI_SHELL_DIALOGS_SELECT_FILE_DIALOG_WIN_H_

#include "ui/gfx/native_widget_types.h"
#include "ui/shell_dialogs/select_file_dialog.h"
#include "ui/shell_dialogs/shell_dialogs_export.h"

namespace ui {
class SelectFilePolicy;

SHELL_DIALOGS_EXPORT std::wstring AppendExtensionIfNeeded(
    const std::wstring& filename,
    const std::wstring& filter_selected,
    const std::wstring& suggested_ext);

SelectFileDialog* CreateWinSelectFileDialog(
    SelectFileDialog::Listener* listener,
    SelectFilePolicy* policy);

}  

#endif  

