// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_CONFIRM_BUBBLE_GTK_H_
#define CHROME_BROWSER_UI_GTK_CONFIRM_BUBBLE_GTK_H_

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/gtk/bubble/bubble_gtk.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/gfx/native_widget_types.h"

typedef struct _GtkWidget GtkWidget;

class ConfirmBubbleModel;
class CustomDrawButton;

class ConfirmBubbleGtk : public BubbleDelegateGtk {
 public:
  ConfirmBubbleGtk(gfx::NativeView parent,
                   const gfx::Point& anchor_point,
                   ConfirmBubbleModel* model);
  virtual ~ConfirmBubbleGtk();

  
  virtual void BubbleClosing(BubbleGtk* bubble, bool closed_by_escape) OVERRIDE;

  
  void Show();

 private:
  FRIEND_TEST_ALL_PREFIXES(ConfirmBubbleGtkTest, ClickCancel);
  FRIEND_TEST_ALL_PREFIXES(ConfirmBubbleGtkTest, ClickOk);
  FRIEND_TEST_ALL_PREFIXES(ConfirmBubbleGtkTest, ClickLink);

  
  CHROMEGTK_CALLBACK_0(ConfirmBubbleGtk, void, OnDestroy);
  CHROMEGTK_CALLBACK_0(ConfirmBubbleGtk, void, OnCloseButton);
  CHROMEGTK_CALLBACK_0(ConfirmBubbleGtk, void, OnOkButton);
  CHROMEGTK_CALLBACK_0(ConfirmBubbleGtk, void, OnCancelButton);
  CHROMEGTK_CALLBACK_0(ConfirmBubbleGtk, void, OnLinkButton);

  
  BubbleGtk* bubble_;

  
  
  gfx::NativeView anchor_;
  gfx::Point anchor_point_;

  
  scoped_ptr<ConfirmBubbleModel> model_;

  
  scoped_ptr<CustomDrawButton> close_button_;

  DISALLOW_COPY_AND_ASSIGN(ConfirmBubbleGtk);
};

#endif  
