// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_COCOA_SETTINGS_CONTROLLER_H_
#define UI_MESSAGE_CENTER_COCOA_SETTINGS_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#import "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#import "ui/message_center/cocoa/settings_entry_view.h"
#include "ui/message_center/message_center_export.h"
#include "ui/message_center/notifier_settings.h"

@class MCSettingsController;
@class MCTrayViewController;

namespace message_center {

class NotifierSettingsObserverMac : public NotifierSettingsObserver {
 public:
  NotifierSettingsObserverMac(MCSettingsController* settings_controller)
      : settings_controller_(settings_controller) {}
  virtual ~NotifierSettingsObserverMac();

  
  virtual void UpdateIconImage(const NotifierId& notifier_id,
                               const gfx::Image& icon) OVERRIDE;
  virtual void NotifierGroupChanged() OVERRIDE;

 private:
  MCSettingsController* settings_controller_;  

  DISALLOW_COPY_AND_ASSIGN(NotifierSettingsObserverMac);
};

}  

MESSAGE_CENTER_EXPORT
@interface MCSettingsController : NSViewController {
 @private
  scoped_ptr<message_center::NotifierSettingsObserverMac> observer_;
  message_center::NotifierSettingsProvider* provider_;
  MCTrayViewController* trayViewController_;  

  
  base::scoped_nsobject<NSTextField> settingsText_;

  
  base::scoped_nsobject<NSTextField> detailsText_;

  
  base::scoped_nsobject<NSPopUpButton> groupDropDownButton_;

  
  base::scoped_nsobject<NSScrollView> scrollView_;

  std::vector<message_center::Notifier*> notifiers_;
}

- (id)initWithProvider:(message_center::NotifierSettingsProvider*)provider
    trayViewController:(MCTrayViewController*)trayViewController;

- (BOOL)notifierHasAdvancedSettings:(const message_center::NotifierId&)id;

- (void)setSettingsNotifier:(message_center::Notifier*)notifier
                    enabled:(BOOL)enabled;

- (void)learnMoreClicked:(message_center::Notifier*)notifier;

@end


@interface MCSettingsController (TestingAPI)
- (NSPopUpButton*)groupDropDownButton;
- (NSScrollView*)scrollView;
@end

#endif  
