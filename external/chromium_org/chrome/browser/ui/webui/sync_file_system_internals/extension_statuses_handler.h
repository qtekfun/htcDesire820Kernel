// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_SYNC_FILE_SYSTEM_INTERNALS_EXTENSION_STATUSES_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_SYNC_FILE_SYSTEM_INTERNALS_EXTENSION_STATUSES_HANDLER_H_

#include "base/compiler_specific.h"
#include "content/public/browser/web_ui_message_handler.h"

class Profile;

namespace syncfs_internals {

class ExtensionStatusesHandler : public content::WebUIMessageHandler {
 public:
  explicit ExtensionStatusesHandler(Profile* profile);
  virtual ~ExtensionStatusesHandler();

  
  
  static void GetExtensionStatusesAsDictionary(Profile* profile,
                                               base::ListValue* values);

  
  virtual void RegisterMessages() OVERRIDE;

 private:
  void GetExtensionStatuses(const base::ListValue* args);

  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionStatusesHandler);
};

}  

#endif  
