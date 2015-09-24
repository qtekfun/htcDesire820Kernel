// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_DEV_MODE_BUBBLE_CONTROLLER_H_
#define CHROME_BROWSER_EXTENSIONS_DEV_MODE_BUBBLE_CONTROLLER_H_

#include <string>
#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "chrome/browser/extensions/extension_message_bubble_controller.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "extensions/common/extension.h"

class Browser;
class ExtensionService;

namespace extensions {

class DevModeBubble;

class DevModeBubbleController
    : public ProfileKeyedAPI,
      public ExtensionMessageBubbleController,
      public ExtensionMessageBubbleController::Delegate {
 public:
  explicit DevModeBubbleController(Profile* profile);
  virtual ~DevModeBubbleController();

  
  static ProfileKeyedAPIFactory<
      DevModeBubbleController>* GetFactoryInstance();

  
  
  static DevModeBubbleController* Get(Profile* profile);

  
  bool IsDevModeExtension(const Extension* extension) const;

  
  virtual bool ShouldIncludeExtension(const std::string& extension_id) OVERRIDE;
  virtual void AcknowledgeExtension(
      const std::string& extension_id,
      ExtensionMessageBubbleController::BubbleAction user_action) OVERRIDE;
  virtual void PerformAction(const ExtensionIdList& list) OVERRIDE;
  virtual string16 GetTitle() const OVERRIDE;
  virtual string16 GetMessageBody() const OVERRIDE;
  virtual string16 GetOverflowText(
      const string16& overflow_count) const OVERRIDE;
  virtual string16 GetLearnMoreLabel() const OVERRIDE;
  virtual GURL GetLearnMoreUrl() const OVERRIDE;
  virtual string16 GetActionButtonLabel() const OVERRIDE;
  virtual string16 GetDismissButtonLabel() const OVERRIDE;
  virtual bool ShouldShowExtensionList() const OVERRIDE;
  virtual std::vector<string16> GetExtensions() OVERRIDE;
  virtual void LogExtensionCount(size_t count) OVERRIDE;
  virtual void LogAction(
      ExtensionMessageBubbleController::BubbleAction action) OVERRIDE;

 private:
  friend class ProfileKeyedAPIFactory<DevModeBubbleController>;

  
  static const char* service_name() {
    return "DevModeBubbleController";
  }
  static const bool kServiceRedirectedInIncognito = true;

  
  ExtensionService* service_;

  
  Profile* profile_;


  DISALLOW_COPY_AND_ASSIGN(DevModeBubbleController);
};

template <>
void ProfileKeyedAPIFactory<
    DevModeBubbleController>::DeclareFactoryDependencies();

}  

#endif  
