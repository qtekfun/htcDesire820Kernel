// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_APP_LIST_VIEW_H_
#define UI_APP_LIST_VIEWS_APP_LIST_VIEW_H_

#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "ui/app_list/app_list_export.h"
#include "ui/app_list/app_list_view_delegate_observer.h"
#include "ui/app_list/speech_ui_model_observer.h"
#include "ui/views/bubble/bubble_delegate.h"
#include "ui/views/widget/widget.h"

namespace base {
class FilePath;
}

namespace app_list {
class ApplicationDragAndDropHost;
class AppListMainView;
class AppListModel;
class AppListViewDelegate;
class AppListViewObserver;
class HideViewAnimationObserver;
class PaginationModel;
class SigninDelegate;
class SigninView;
class SpeechView;

class APP_LIST_EXPORT AppListView : public views::BubbleDelegateView,
                                    public AppListViewDelegateObserver,
                                    public SpeechUIModelObserver {
 public:

  
  explicit AppListView(AppListViewDelegate* delegate);
  virtual ~AppListView();

  
  
  void InitAsBubbleAttachedToAnchor(gfx::NativeView parent,
                                    PaginationModel* pagination_model,
                                    views::View* anchor,
                                    const gfx::Vector2d& anchor_offset,
                                    views::BubbleBorder::Arrow arrow,
                                    bool border_accepts_events);

  
  
  void InitAsBubbleAtFixedLocation(gfx::NativeView parent,
                                   PaginationModel* pagination_model,
                                   const gfx::Point& anchor_point_in_screen,
                                   views::BubbleBorder::Arrow arrow,
                                   bool border_accepts_events);

  void SetBubbleArrow(views::BubbleBorder::Arrow arrow);

  void SetAnchorPoint(const gfx::Point& anchor_point);

  
  
  
  
  void SetDragAndDropHostOfCurrentAppList(
      ApplicationDragAndDropHost* drag_and_drop_host);

  
  
  void ShowWhenReady();

  void Close();

  void UpdateBounds();

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Paint(gfx::Canvas* canvas) OVERRIDE;

  
  virtual bool ShouldHandleSystemCommands() const OVERRIDE;

  
  virtual void OnProfilesChanged() OVERRIDE;

  void Prerender();

  void SetProfileByPath(const base::FilePath& profile_path);

  void AddObserver(AppListViewObserver* observer);
  void RemoveObserver(AppListViewObserver* observer);

  
  static void SetNextPaintCallback(void (*callback)());

#if defined(OS_WIN)
  HWND GetHWND() const;
#endif

  AppListMainView* app_list_main_view() { return app_list_main_view_; }

 private:
  void InitAsBubbleInternal(gfx::NativeView parent,
                            PaginationModel* pagination_model,
                            views::BubbleBorder::Arrow arrow,
                            bool border_accepts_events,
                            const gfx::Vector2d& anchor_offset);

  
  virtual void OnBeforeBubbleWidgetInit(
      views::Widget::InitParams* params,
      views::Widget* widget) const OVERRIDE;

  
  virtual views::View* GetInitiallyFocusedView() OVERRIDE;
  virtual gfx::ImageSkia GetWindowIcon() OVERRIDE;
  virtual bool WidgetHasHitTestMask() const OVERRIDE;
  virtual void GetWidgetHitTestMask(gfx::Path* mask) const OVERRIDE;

  
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;
  virtual void Layout() OVERRIDE;

  
  virtual void OnWidgetDestroying(views::Widget* widget) OVERRIDE;
  virtual void OnWidgetVisibilityChanged(
      views::Widget* widget, bool visible) OVERRIDE;
  virtual void OnWidgetActivationChanged(
      views::Widget* widget, bool active) OVERRIDE;

  
  virtual void OnSpeechRecognitionStateChanged(
      SpeechRecognitionState new_state) OVERRIDE;

  SigninDelegate* GetSigninDelegate();

  scoped_ptr<AppListViewDelegate> delegate_;

  AppListMainView* app_list_main_view_;
  SigninView* signin_view_;
  SpeechView* speech_view_;

  ObserverList<AppListViewObserver> observers_;
  scoped_ptr<HideViewAnimationObserver> animation_observer_;

  DISALLOW_COPY_AND_ASSIGN(AppListView);
};

}  

#endif  
