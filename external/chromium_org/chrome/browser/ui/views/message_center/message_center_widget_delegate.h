// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_MESSAGE_CENTER_MESSAGE_CENTER_WIDGET_DELEGATE_H_
#define CHROME_BROWSER_UI_VIEWS_MESSAGE_CENTER_MESSAGE_CENTER_WIDGET_DELEGATE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/ui/views/message_center/web_notification_tray.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/message_center/message_center.h"
#include "ui/message_center/message_center_tray.h"
#include "ui/message_center/message_center_tray_delegate.h"
#include "ui/message_center/views/message_center_view.h"
#include "ui/views/widget/widget_delegate.h"
#include "ui/views/widget/widget_observer.h"

namespace message_center {

enum Alignment {
  ALIGNMENT_TOP = 1 << 0,
  ALIGNMENT_LEFT = 1 << 1,
  ALIGNMENT_BOTTOM = 1 << 2,
  ALIGNMENT_RIGHT = 1 << 3,
  ALIGNMENT_NONE = 1 << 4,
};

struct PositionInfo {
  int max_height;

  
  Alignment message_center_alignment;

  
  Alignment taskbar_alignment;

  
  gfx::Point inital_anchor_point;
};

class WebNotificationTray;
class MessageCenterFrameView;

class MessageCenterWidgetDelegate : public views::WidgetDelegate,
                                    public message_center::MessageCenterView,
                                    public views::WidgetObserver {
 public:
  MessageCenterWidgetDelegate(WebNotificationTray* tray,
                              MessageCenterTray* mc_tray,
                              bool initially_settings_visible,
                              const PositionInfo& pos_info);
  virtual ~MessageCenterWidgetDelegate();

  
  virtual View* GetContentsView() OVERRIDE;
  virtual views::NonClientFrameView* CreateNonClientFrameView(
      views::Widget* widget) OVERRIDE;
  virtual void DeleteDelegate() OVERRIDE;
  virtual views::Widget* GetWidget() OVERRIDE;
  virtual const views::Widget* GetWidget() const OVERRIDE;

  
  virtual void OnWidgetActivationChanged(views::Widget* widget,
                                         bool active) OVERRIDE;
  virtual void OnWidgetClosing(views::Widget* widget) OVERRIDE;

  
  virtual void PreferredSizeChanged() OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual gfx::Size GetMaximumSize() OVERRIDE;
  virtual int GetHeightForWidth(int width) OVERRIDE;
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;

 private:
  
  void InitWidget();

  
  
  
  
  gfx::Point GetCorrectedAnchor(gfx::Size calculated_size);

  
  
  gfx::Rect GetMessageCenterBounds();

  
  gfx::Insets border_insets_;

  
  PositionInfo pos_info_;

  WebNotificationTray* tray_;
};

}  

#endif  
