// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_LOCATION_BAR_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_LOCATION_BAR_VIEW_H_
#pragma once

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/task.h"
#include "chrome/browser/autocomplete/autocomplete_edit.h"
#include "chrome/browser/extensions/extension_context_menu_model.h"
#include "chrome/browser/first_run/first_run.h"
#include "chrome/browser/prefs/pref_member.h"
#include "chrome/browser/search_engines/template_url_model_observer.h"
#include "chrome/browser/ui/omnibox/location_bar.h"
#include "chrome/browser/ui/toolbar/toolbar_model.h"
#include "chrome/browser/ui/views/extensions/extension_popup.h"
#include "ui/gfx/font.h"
#include "ui/gfx/rect.h"
#include "views/controls/native/native_view_host.h"

#if defined(OS_WIN)
#include "chrome/browser/autocomplete/autocomplete_edit_view_win.h"
#elif defined(OS_LINUX)
#include "chrome/browser/autocomplete/autocomplete_edit_view_gtk.h"
#endif

class CommandUpdater;
class ContentSettingImageView;
class EVBubbleView;
class ExtensionAction;
class GURL;
class InstantController;
class KeywordHintView;
class LocationIconView;
class PageActionWithBadgeView;
class Profile;
class SelectedKeywordView;
class StarView;
class TabContents;
class TabContentsWrapper;
class TemplateURLModel;

namespace views {
class HorizontalPainter;
class Label;
};

#if defined(OS_WIN)
class SuggestedTextView;
#endif

class LocationBarView : public LocationBar,
                        public LocationBarTesting,
                        public views::View,
                        public views::DragController,
                        public AutocompleteEditController,
                        public TemplateURLModelObserver,
                        public NotificationObserver {
 public:
  
  static const char kViewClassName[];

  class Delegate {
   public:
    
    virtual TabContentsWrapper* GetTabContentsWrapper() const = 0;

    
    virtual InstantController* GetInstant() = 0;

    
    
    
    virtual void OnInputInProgress(bool in_progress) = 0;
  };

  enum ColorKind {
    BACKGROUND = 0,
    TEXT,
    SELECTED_TEXT,
    DEEMPHASIZED_TEXT,
    SECURITY_TEXT,
  };

  
  
  
  
  
  
  enum Mode {
    NORMAL = 0,
    POPUP,
    APP_LAUNCHER
  };

  LocationBarView(Profile* profile,
                  CommandUpdater* command_updater,
                  ToolbarModel* model,
                  Delegate* delegate,
                  Mode mode);
  virtual ~LocationBarView();

  void Init();

  
  
  bool IsInitialized() const;

  
  
  static SkColor GetColor(ToolbarModel::SecurityLevel security_level,
                          ColorKind kind);

  
  
  
  void Update(const TabContents* tab_for_state_restoring);

  void SetProfile(Profile* profile);
  Profile* profile() const { return profile_; }

  
  
  
  
  
  void SetPreviewEnabledPageAction(ExtensionAction *page_action,
                                   bool preview_enabled);

  
  views::View* GetPageActionView(ExtensionAction* page_action);

  
  void SetStarToggled(bool on);

  
  void ShowStarBubble(const GURL& url, bool newly_bookmarked);

  
  
  gfx::Point GetLocationEntryOrigin() const;

#if defined(OS_WIN)
  
  void SetInstantSuggestion(const string16& text,
                            bool animate_to_complete);

  
  string16 GetInstantSuggestion() const;
#endif

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;

  
  virtual void Layout() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  
  virtual void OnPaintFocusBorder(gfx::Canvas* canvas) OVERRIDE { }

  
  
  
  virtual void SetShowFocusRect(bool show);

  
  
  virtual void SelectAll();

#if defined(OS_WIN)
  
  virtual bool OnMousePressed(const views::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const views::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;
#endif

  const LocationIconView* location_icon_view() const {
    return location_icon_view_;
  }

  
  virtual void OnAutocompleteAccept(const GURL& url,
                                    WindowOpenDisposition disposition,
                                    PageTransition::Type transition,
                                    const GURL& alternate_nav_url) OVERRIDE;
  virtual void OnChanged() OVERRIDE;
  virtual void OnSelectionBoundsChanged() OVERRIDE;
  virtual void OnInputInProgress(bool in_progress) OVERRIDE;
  virtual void OnKillFocus() OVERRIDE;
  virtual void OnSetFocus() OVERRIDE;
  virtual SkBitmap GetFavicon() const OVERRIDE;
  virtual string16 GetTitle() const OVERRIDE;
  virtual InstantController* GetInstant() OVERRIDE;
  virtual TabContentsWrapper* GetTabContentsWrapper() const OVERRIDE;

  
  virtual std::string GetClassName() const OVERRIDE;
  virtual bool SkipDefaultKeyEventProcessing(const views::KeyEvent& event)
      OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual void WriteDragDataForView(View* sender,
                                    const gfx::Point& press_pt,
                                    OSExchangeData* data) OVERRIDE;
  virtual int GetDragOperationsForView(View* sender,
                                       const gfx::Point& p) OVERRIDE;
  virtual bool CanStartDragForView(View* sender,
                                   const gfx::Point& press_pt,
                                   const gfx::Point& p) OVERRIDE;

  
  virtual void ShowFirstRunBubble(FirstRun::BubbleType bubble_type) OVERRIDE;
  virtual void SetSuggestedText(const string16& text,
                                InstantCompleteBehavior behavior) OVERRIDE;
  virtual std::wstring GetInputString() const OVERRIDE;
  virtual WindowOpenDisposition GetWindowOpenDisposition() const OVERRIDE;
  virtual PageTransition::Type GetPageTransition() const OVERRIDE;
  virtual void AcceptInput() OVERRIDE;
  virtual void FocusLocation(bool select_all) OVERRIDE;
  virtual void FocusSearch() OVERRIDE;
  virtual void UpdateContentSettingsIcons() OVERRIDE;
  virtual void UpdatePageActions() OVERRIDE;
  virtual void InvalidatePageActions() OVERRIDE;
  virtual void SaveStateToContents(TabContents* contents) OVERRIDE;
  virtual void Revert() OVERRIDE;
  virtual const AutocompleteEditView* location_entry() const OVERRIDE;
  virtual AutocompleteEditView* location_entry() OVERRIDE;
  virtual LocationBarTesting* GetLocationBarForTesting() OVERRIDE;

  
  virtual int PageActionCount() OVERRIDE;
  virtual int PageActionVisibleCount() OVERRIDE;
  virtual ExtensionAction* GetPageAction(size_t index) OVERRIDE;
  virtual ExtensionAction* GetVisiblePageAction(size_t index) OVERRIDE;
  virtual void TestPageActionPressed(size_t index) OVERRIDE;

  
  virtual void OnTemplateURLModelChanged() OVERRIDE;

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  
  static const int kNormalHorizontalEdgeThickness;
  
  static const int kVerticalEdgeThickness;
  
  static const int kItemPadding;
  
  static const int kIconInternalPadding;
  
  static const int kEdgeItemPadding;
  
  static const int kBubbleHorizontalPadding;

 protected:
  virtual void OnFocus() OVERRIDE;

 private:
  typedef std::vector<ContentSettingImageView*> ContentSettingViews;

  friend class PageActionImageView;
  friend class PageActionWithBadgeView;
  typedef std::vector<PageActionWithBadgeView*> PageActionViews;

  
  
  
  int AvailableWidth(int location_bar_width);

  
  
  
  
  
  void LayoutView(views::View* view,
                  int padding,
                  int available_width,
                  bool leading,
                  gfx::Rect* bounds);

  
  
  void RefreshContentSettingViews();

  
  void DeletePageActionViews();

  
  
  void RefreshPageActionViews();

  
  void ToggleVisibility(bool new_vis, views::View* view);

#if defined(OS_WIN)
  
  void OnMouseEvent(const views::MouseEvent& event, UINT msg);

  
  bool HasValidSuggestText() const;
#endif

  
  void ShowFirstRunBubbleInternal(FirstRun::BubbleType bubble_type);

  
  Profile* profile_;

  
#if defined(OS_WIN)
  scoped_ptr<AutocompleteEditViewWin> location_entry_;
#else
  scoped_ptr<AutocompleteEditView> location_entry_;
#endif

  
  CommandUpdater* command_updater_;

  
  ToolbarModel* model_;

  
  Delegate* delegate_;

  
  
  std::wstring location_input_;

  
  WindowOpenDisposition disposition_;

  
  PageTransition::Type transition_;

  
  gfx::Font font_;

  
  scoped_ptr<views::HorizontalPainter> painter_;

  
  LocationIconView* location_icon_view_;

  
  EVBubbleView* ev_bubble_view_;

  
  views::View* location_entry_view_;

  
  
  
  
  

  
  SelectedKeywordView* selected_keyword_view_;

#if defined(OS_WIN)
  
  
  SuggestedTextView* suggested_text_view_;
#endif

  
  KeywordHintView* keyword_hint_view_;

  
  ContentSettingViews content_setting_views_;

  
  PageActionViews page_action_views_;

  
  StarView* star_view_;

  
  Mode mode_;

  
  
  bool show_focus_rect_;

  
  FirstRun::BubbleType bubble_type_;

  
  
  
  TemplateURLModel* template_url_model_;

  
  BooleanPrefMember edit_bookmarks_enabled_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(LocationBarView);
};

#endif  
