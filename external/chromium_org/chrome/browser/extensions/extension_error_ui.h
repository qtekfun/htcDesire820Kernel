// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_ERROR_UI_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_ERROR_UI_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/ui/global_error/global_error.h"
#include "extensions/common/extension.h"

class Browser;
class ExtensionService;

class ExtensionErrorUI {
 public:
  static ExtensionErrorUI* Create(ExtensionService* extension_service);

  virtual ~ExtensionErrorUI();

  
  
  void AddExternalExtension(const std::string& id);
  void AddBlacklistedExtension(const std::string& id);

  
  
  const extensions::ExtensionIdSet* get_external_extension_ids() const {
    return external_extension_ids_.get();
  }

  const extensions::ExtensionIdSet* get_blacklisted_extension_ids() const {
    return blacklisted_extension_ids_.get();
  }

  
  
  virtual bool ShowErrorInBubbleView() = 0;

  
  
  
  
  virtual void ShowExtensions() = 0;

  
  
  virtual void Close() = 0;

 protected:
  explicit ExtensionErrorUI(ExtensionService* extension_service);

  ExtensionService* extension_service() const { return extension_service_; }

  
  base::string16 GetBubbleViewTitle();
  std::vector<base::string16> GetBubbleViewMessages();
  base::string16 GetBubbleViewAcceptButtonLabel();
  base::string16 GetBubbleViewCancelButtonLabel();

  
  
  void BubbleViewDidClose();  
  void BubbleViewAcceptButtonPressed();
  void BubbleViewCancelButtonPressed();

 private:
  bool should_delete_self_on_close_;
  ExtensionService* extension_service_;
  scoped_ptr<extensions::ExtensionIdSet> external_extension_ids_;
  scoped_ptr<extensions::ExtensionIdSet> blacklisted_extension_ids_;
  base::string16 message_;  

  
  
  base::string16 GenerateMessageSection(
      const extensions::ExtensionIdSet* extensions,
      int extension_template_message_id,
      int app_template_message_id);

  
  base::string16 GenerateMessage();

  DISALLOW_COPY_AND_ASSIGN(ExtensionErrorUI);
};

#endif  
