// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_VIEWS_TOAST_CONTENTS_VIEW_H_
#define UI_MESSAGE_CENTER_VIEWS_TOAST_CONTENTS_VIEW_H_

#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"
#include "ui/message_center/views/message_center_controller.h"
#include "ui/views/widget/widget_delegate.h"

namespace gfx {
class Animation;
class SlideAnimation;
}

namespace views {
class View;
}

namespace message_center {

class MessagePopupCollection;
class MessageView;
class Notification;

class ToastContentsView : public views::WidgetDelegateView,
                          public MessageCenterController,
                          public gfx::AnimationDelegate {
 public:
  
  static gfx::Size GetToastSizeForView(views::View* view);

  ToastContentsView(const std::string& notification_id,
                    base::WeakPtr<MessagePopupCollection> collection);
  virtual ~ToastContentsView();

  
  
  
  void SetContents(MessageView* view, bool a11y_feedback_for_updates);

  
  
  void RevealWithAnimation(gfx::Point origin);

  
  
  void CloseWithAnimation();

  void SetBoundsWithAnimation(gfx::Rect new_bounds);

  
  
  gfx::Point origin() { return origin_; }
  gfx::Rect bounds() { return gfx::Rect(origin_, preferred_size_); }

  const std::string& id() { return id_; }

  
  virtual void OnMouseEntered(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

 private:
  
  virtual void ClickOnNotification(const std::string& notification_id) OVERRIDE;
  virtual void RemoveNotification(const std::string& notification_id,
                                  bool by_user) OVERRIDE;
  virtual void DisableNotificationsFromThisSource(
      const NotifierId& notifier_id) OVERRIDE;
  virtual void ShowNotifierSettingsBubble() OVERRIDE;
  virtual bool HasClickedListener(const std::string& notification_id) OVERRIDE;
  virtual void ClickOnNotificationButton(const std::string& notification_id,
                                         int button_index) OVERRIDE;
  virtual void ExpandNotification(const std::string& notification_id) OVERRIDE;
  virtual void GroupBodyClicked(const std::string& last_notification_id)
      OVERRIDE;
  virtual void ExpandGroup(const NotifierId& notifier_id) OVERRIDE;
  virtual void RemoveGroup(const NotifierId& notifier_id) OVERRIDE;

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationCanceled(const gfx::Animation* animation) OVERRIDE;

  
  virtual views::View* GetContentsView() OVERRIDE;
  virtual void WindowClosing() OVERRIDE;
  virtual bool CanActivate() const OVERRIDE;
  virtual void OnDisplayChanged() OVERRIDE;
  virtual void OnWorkAreaChanged() OVERRIDE;

  
  void CreateWidget(gfx::NativeView parent);

  
  void SetBoundsInstantly(gfx::Rect new_bounds);

  
  
  
  gfx::Rect GetClosedToastBounds(gfx::Rect bounds);

  void StartFadeIn();
  void StartFadeOut();  
  void OnBoundsAnimationEndedOrCancelled(const gfx::Animation* animation);

  base::WeakPtr<MessagePopupCollection> collection_;

  
  std::string id_;

  scoped_ptr<gfx::SlideAnimation> bounds_animation_;
  scoped_ptr<gfx::SlideAnimation> fade_animation_;

  bool is_animating_bounds_;
  gfx::Rect animated_bounds_start_;
  gfx::Rect animated_bounds_end_;
  
  bool is_closing_;
  
  
  gfx::Animation* closing_animation_;

  gfx::Point origin_;
  gfx::Size preferred_size_;

  DISALLOW_COPY_AND_ASSIGN(ToastContentsView);
};

}  

#endif 
