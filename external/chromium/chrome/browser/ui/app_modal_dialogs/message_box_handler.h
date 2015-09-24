// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_MODAL_DIALOGS_MESSAGE_BOX_HANDLER_H_
#define CHROME_BROWSER_UI_APP_MODAL_DIALOGS_MESSAGE_BOX_HANDLER_H_
#pragma once

#include <string>

#include "ipc/ipc_message.h"

class GURL;
class JavaScriptAppModalDialogDelegate;
class TabContents;
class Profile;

void RunJavascriptMessageBox(Profile* profile,
                             JavaScriptAppModalDialogDelegate* delegate,
                             const GURL& frame_url,
                             int dialog_flags,
                             const std::wstring& message_text,
                             const std::wstring& default_prompt_text,
                             bool display_suppress_checkbox,
                             IPC::Message* reply_msg);

void RunBeforeUnloadDialog(TabContents* tab_contents,
                           const std::wstring& message_text,
                           IPC::Message* reply_msg);

#endif  
