// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>
#import <PreferencePanes/PreferencePanes.h>
#import <SecurityInterface/SFAuthorizationView.h>

#include <string>

#include "base/memory/scoped_ptr.h"
#include "third_party/jsoncpp/source/include/json/value.h"

namespace remoting {


class JsonHostConfig {
 public:
  JsonHostConfig(const std::string& filename);
  ~JsonHostConfig();

  bool Read();
  bool GetString(const std::string& path, std::string* out_value) const;
  std::string GetSerializedData() const;

 private:
  Json::Value config_;
  std::string filename_;

  DISALLOW_COPY_AND_ASSIGN(JsonHostConfig);
};

}

@class Me2MePreferencePaneConfirmPin;
@class Me2MePreferencePaneDisable;

@interface Me2MePreferencePane : NSPreferencePane {
  Me2MePreferencePaneConfirmPin* confirm_pin_view_;
  Me2MePreferencePaneDisable* disable_view_;

  IBOutlet NSTextField* status_message_;
  IBOutlet NSBox* box_;
  IBOutlet SFAuthorizationView* authorization_view_;

  
  
  scoped_ptr<remoting::JsonHostConfig> config_;

  NSTimer* service_status_timer_;

  
  
  BOOL is_service_running_;
  BOOL is_pane_unlocked_;

  
  BOOL have_new_config_;

  
  
  BOOL awaiting_service_stop_;

  
  
  
  BOOL restart_pending_or_canceled_;
}

- (void)mainViewDidLoad;
- (void)willSelect;
- (void)didSelect;
- (void)willUnselect;
- (void)onDisable:(id)sender;
- (void)applyConfiguration:(id)sender
                       pin:(NSString*)pin;
- (void)onNewConfigFile:(NSNotification*)notification;
- (void)refreshServiceStatus:(NSTimer*)timer;
- (void)authorizationViewDidAuthorize:(SFAuthorizationView*)view;
- (void)authorizationViewDidDeauthorize:(SFAuthorizationView*)view;
- (void)updateServiceStatus;
- (void)updateAuthorizationStatus;

- (void)readNewConfig;

- (void)updateUI;

- (void)showError;

- (void)showIncorrectPinMessage;

- (void)applyNewServiceConfig;

- (BOOL)runHelperAsRootWithCommand:(const char*)command
                         inputData:(const std::string&)input_data;
- (BOOL)sendJobControlMessage:(const char*)launch_key;

- (void)checkInstalledVersion;

- (void)mismatchAlertDidEnd:(NSAlert*)alert
                 returnCode:(NSInteger)returnCode
                contextInfo:(void*)contextInfo;

- (void)restartSystemPreferences;

@end
