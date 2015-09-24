// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_GAIA_SCREEN_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_GAIA_SCREEN_HANDLER_H_

#include "base/basictypes.h"
#include "base/command_line.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/ui/webui/chromeos/login/network_state_informer.h"
#include "net/base/net_errors.h"

namespace chromeos {

class SigninScreenHandler;

struct GaiaContext {
  GaiaContext();

  
  bool force_reload;

  
  bool is_local;

  
  bool password_changed;

  
  bool show_users;

  
  bool use_offline;

  
  bool has_users;

  
  std::string email;
};

class GaiaScreenHandler : public BaseScreenHandler {
 public:
  enum FrameState {
    FRAME_STATE_UNKNOWN = 0,
    FRAME_STATE_LOADING,
    FRAME_STATE_LOADED,
    FRAME_STATE_ERROR
  };

  explicit GaiaScreenHandler(
      const scoped_refptr<NetworkStateInformer>& network_state_informer);
  virtual ~GaiaScreenHandler();

  void LoadGaia(const GaiaContext& context);
  void UpdateGaia(const GaiaContext& context);
  void ReloadGaia();

  FrameState frame_state() const { return frame_state_; }
  net::Error frame_error() const { return frame_error_; }

 private:
  
  friend class SigninScreenHandler;

  
  virtual void DeclareLocalizedValues(LocalizedValuesBuilder* builder) OVERRIDE;
  virtual void Initialize() OVERRIDE;

  
  virtual void RegisterMessages() OVERRIDE;

  
  void HandleFrameLoadingCompleted(int status);

  
  
  void UpdateState(ErrorScreenActor::ErrorReason reason);

  
  void SetSigninScreenHandler(SigninScreenHandler* handler);

  
  FrameState frame_state_;

  
  net::Error frame_error_;

  
  scoped_refptr<NetworkStateInformer> network_state_informer_;

  
  
  
  
  SigninScreenHandler* signin_screen_handler_;

  DISALLOW_COPY_AND_ASSIGN(GaiaScreenHandler);
};

}  

#endif  
