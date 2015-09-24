// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_CONFIRM_BUBBLE_VIEWS_H_
#define CHROME_BROWSER_UI_VIEWS_CONFIRM_BUBBLE_VIEWS_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ui/views/controls/link_listener.h"
#include "ui/views/window/dialog_delegate.h"

class ConfirmBubbleModel;

class ConfirmBubbleViews : public views::DialogDelegateView,
                           public views::LinkListener {
 public:
  explicit ConfirmBubbleViews(ConfirmBubbleModel* model);

 protected:
  virtual ~ConfirmBubbleViews();

  
  virtual base::string16 GetDialogButtonLabel(
      ui::DialogButton button) const OVERRIDE;
  virtual bool IsDialogButtonEnabled(ui::DialogButton button) const OVERRIDE;
  virtual views::View* CreateExtraView() OVERRIDE;
  virtual bool Cancel() OVERRIDE;
  virtual bool Accept() OVERRIDE;

  
  virtual ui::ModalType GetModalType() const OVERRIDE;
  virtual base::string16 GetWindowTitle() const OVERRIDE;

  
  virtual void LinkClicked(views::Link* source, int event_flags) OVERRIDE;

 private:
  
  scoped_ptr<ConfirmBubbleModel> model_;

  views::Link* link_;

  DISALLOW_COPY_AND_ASSIGN(ConfirmBubbleViews);
};

#endif  
