// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTERNAL_PROTOCOL_DIALOG_H_
#define CHROME_BROWSER_CHROMEOS_EXTERNAL_PROTOCOL_DIALOG_H_
#pragma once

#include "base/time.h"
#include "views/window/dialog_delegate.h"

class GURL;
class TabContents;

namespace views {
class MessageBoxView;
}

class ExternalProtocolDialog : public views::DialogDelegate {
 public:
  
  ExternalProtocolDialog(TabContents* tab_contents, const GURL& url);

  virtual ~ExternalProtocolDialog();

  
  virtual int GetDialogButtons() const;
  virtual std::wstring GetDialogButtonLabel(
      MessageBoxFlags::DialogButton button) const;
  virtual std::wstring GetWindowTitle() const;
  virtual void DeleteDelegate();
  virtual bool Accept();
  virtual views::View* GetContentsView();

  
  virtual bool IsAlwaysOnTop() const { return false; }
  virtual bool IsModal() const { return false; }

 private:
  
  views::MessageBoxView* message_box_view_;

  
  base::TimeTicks creation_time_;

  
  std::string scheme_;

  DISALLOW_COPY_AND_ASSIGN(ExternalProtocolDialog);
};

#endif  
