// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTERNAL_PROTOCOL_DIALOG_H_
#define CHROME_BROWSER_CHROMEOS_EXTERNAL_PROTOCOL_DIALOG_H_

#include <string>

#include "base/strings/string16.h"
#include "base/time/time.h"
#include "ui/views/window/dialog_delegate.h"

class GURL;

namespace content {
class WebContents;
}

namespace views {
class MessageBoxView;
}

class ExternalProtocolDialog : public views::DialogDelegate {
 public:
  
  ExternalProtocolDialog(content::WebContents* web_contents, const GURL& url);

  virtual ~ExternalProtocolDialog();

  
  virtual int GetDialogButtons() const OVERRIDE;
  virtual base::string16 GetDialogButtonLabel(
      ui::DialogButton button) const OVERRIDE;
  virtual base::string16 GetWindowTitle() const OVERRIDE;
  virtual void DeleteDelegate() OVERRIDE;
  virtual bool Accept() OVERRIDE;
  virtual views::View* GetContentsView() OVERRIDE;

  
  virtual const views::Widget* GetWidget() const OVERRIDE;
  virtual views::Widget* GetWidget() OVERRIDE;

 private:
  
  views::MessageBoxView* message_box_view_;

  
  base::TimeTicks creation_time_;

  
  std::string scheme_;

  DISALLOW_COPY_AND_ASSIGN(ExternalProtocolDialog);
};

#endif  
