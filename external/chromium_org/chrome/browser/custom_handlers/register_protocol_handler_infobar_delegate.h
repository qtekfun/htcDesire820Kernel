// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CUSTOM_HANDLERS_REGISTER_PROTOCOL_HANDLER_INFOBAR_DELEGATE_H_
#define CHROME_BROWSER_CUSTOM_HANDLERS_REGISTER_PROTOCOL_HANDLER_INFOBAR_DELEGATE_H_

#include "base/strings/string16.h"
#include "chrome/browser/infobars/confirm_infobar_delegate.h"
#include "chrome/common/custom_handlers/protocol_handler.h"

class InfoBarService;
class ProtocolHandlerRegistry;

class RegisterProtocolHandlerInfoBarDelegate : public ConfirmInfoBarDelegate {
 public:
  
  
  
  
  static void Create(InfoBarService* infobar_service,
                     ProtocolHandlerRegistry* registry,
                     const ProtocolHandler& handler);

 private:
  RegisterProtocolHandlerInfoBarDelegate(ProtocolHandlerRegistry* registry,
                                         const ProtocolHandler& handler);
  virtual ~RegisterProtocolHandlerInfoBarDelegate();

  
  virtual InfoBarAutomationType GetInfoBarAutomationType() const OVERRIDE;
  virtual Type GetInfoBarType() const OVERRIDE;
  virtual RegisterProtocolHandlerInfoBarDelegate*
      AsRegisterProtocolHandlerInfoBarDelegate() OVERRIDE;
  virtual base::string16 GetMessageText() const OVERRIDE;
  virtual base::string16 GetButtonLabel(InfoBarButton button) const OVERRIDE;
  virtual bool NeedElevation(InfoBarButton button) const OVERRIDE;
  virtual bool Accept() OVERRIDE;
  virtual bool Cancel() OVERRIDE;
  virtual base::string16 GetLinkText() const OVERRIDE;
  virtual bool LinkClicked(WindowOpenDisposition disposition) OVERRIDE;

  
  base::string16 GetProtocolName(const ProtocolHandler& handler) const;

  ProtocolHandlerRegistry* registry_;
  ProtocolHandler handler_;

  DISALLOW_COPY_AND_ASSIGN(RegisterProtocolHandlerInfoBarDelegate);
};

#endif  
