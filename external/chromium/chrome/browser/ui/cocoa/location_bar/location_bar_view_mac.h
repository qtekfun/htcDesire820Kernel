// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_LOCATION_BAR_VIEW_MAC_H_
#define CHROME_BROWSER_UI_COCOA_LOCATION_BAR_VIEW_MAC_H_
#pragma once

#include <string>

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "chrome/browser/autocomplete/autocomplete_edit.h"
#include "chrome/browser/autocomplete/autocomplete_edit_view_mac.h"
#include "chrome/browser/extensions/image_loading_tracker.h"
#include "chrome/browser/first_run/first_run.h"
#include "chrome/browser/prefs/pref_member.h"
#include "chrome/browser/ui/omnibox/location_bar.h"
#include "chrome/browser/ui/toolbar/toolbar_model.h"
#include "chrome/common/content_settings_types.h"

@class AutocompleteTextField;
class CommandUpdater;
class ContentSettingDecoration;
class ContentSettingImageModel;
class EVBubbleDecoration;
@class ExtensionPopupController;
class KeywordHintDecoration;
class LocationIconDecoration;
class PageActionDecoration;
class Profile;
class SelectedKeywordDecoration;
class SkBitmap;
class StarDecoration;
class ToolbarModel;


class LocationBarViewMac : public AutocompleteEditController,
                           public LocationBar,
                           public LocationBarTesting,
                           public NotificationObserver {
 public:
  LocationBarViewMac(AutocompleteTextField* field,
                     CommandUpdater* command_updater,
                     ToolbarModel* toolbar_model,
                     Profile* profile,
                     Browser* browser);
  virtual ~LocationBarViewMac();

  
  virtual void ShowFirstRunBubble(FirstRun::BubbleType bubble_type);
  virtual void SetSuggestedText(const string16& text,
                                InstantCompleteBehavior behavior);
  virtual std::wstring GetInputString() const;
  virtual WindowOpenDisposition GetWindowOpenDisposition() const;
  virtual PageTransition::Type GetPageTransition() const;
  virtual void AcceptInput();
  virtual void FocusLocation(bool select_all);
  virtual void FocusSearch();
  virtual void UpdateContentSettingsIcons();
  virtual void UpdatePageActions();
  virtual void InvalidatePageActions();
  virtual void SaveStateToContents(TabContents* contents);
  virtual void Revert();
  virtual const AutocompleteEditView* location_entry() const;
  virtual AutocompleteEditView* location_entry();
  virtual LocationBarTesting* GetLocationBarForTesting();

  
  virtual int PageActionCount();
  virtual int PageActionVisibleCount();
  virtual ExtensionAction* GetPageAction(size_t index);
  virtual ExtensionAction* GetVisiblePageAction(size_t index);
  virtual void TestPageActionPressed(size_t index);

  
  void SetEditable(bool editable);
  bool IsEditable();

  
  void SetStarred(bool starred);

  
  NSPoint GetBookmarkBubblePoint() const;

  
  NSPoint GetPageInfoBubblePoint() const;

  
  NSPoint GetFirstRunBubblePoint() const;

  
  
  
  void Update(const TabContents* tab, bool should_restore_state);

  
  void Layout();

  
  TabContents* GetTabContents() const;

  
  
  
  
  
  void SetPreviewEnabledPageAction(ExtensionAction* page_action,
                                   bool preview_enabled);

  
  
  
  
  NSPoint GetPageActionBubblePoint(ExtensionAction* page_action);

  
  
  
  
  NSRect GetBlockedPopupRect() const;

  
  virtual void OnAutocompleteAccept(const GURL& url,
      WindowOpenDisposition disposition,
      PageTransition::Type transition,
      const GURL& alternate_nav_url);
  virtual void OnChanged();
  virtual void OnSelectionBoundsChanged();
  virtual void OnInputInProgress(bool in_progress);
  virtual void OnKillFocus();
  virtual void OnSetFocus();
  virtual SkBitmap GetFavicon() const;
  virtual string16 GetTitle() const;
  virtual InstantController* GetInstant();
  virtual TabContentsWrapper* GetTabContentsWrapper() const;

  NSImage* GetKeywordImage(const string16& keyword);

  AutocompleteTextField* GetAutocompleteTextField() { return field_; }


  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  
  void PostNotification(NSString* notification);

  
  PageActionDecoration* GetPageActionDecoration(ExtensionAction* page_action);

  
  void DeletePageActionDecorations();

  
  
  void RefreshPageActionDecorations();

  
  
  bool RefreshContentSettingsDecorations();

  void ShowFirstRunBubbleInternal(FirstRun::BubbleType bubble_type);

  
  bool IsStarEnabled();

  scoped_ptr<AutocompleteEditViewMac> edit_view_;

  CommandUpdater* command_updater_;  

  AutocompleteTextField* field_;  

  
  
  
  std::wstring location_input_;

  
  WindowOpenDisposition disposition_;

  
  scoped_ptr<LocationIconDecoration> location_icon_decoration_;

  
  scoped_ptr<SelectedKeywordDecoration> selected_keyword_decoration_;

  
  
  scoped_ptr<EVBubbleDecoration> ev_bubble_decoration_;

  
  scoped_ptr<StarDecoration> star_decoration_;

  
  ScopedVector<PageActionDecoration> page_action_decorations_;

  
  ScopedVector<ContentSettingDecoration> content_setting_decorations_;

  
  scoped_ptr<KeywordHintDecoration> keyword_hint_decoration_;

  Profile* profile_;

  Browser* browser_;

  ToolbarModel* toolbar_model_;  

  
  PageTransition::Type transition_;

  
  NotificationRegistrar registrar_;

  
  ScopedRunnableMethodFactory<LocationBarViewMac> first_run_bubble_;

  
  BooleanPrefMember edit_bookmarks_enabled_;

  DISALLOW_COPY_AND_ASSIGN(LocationBarViewMac);
};

#endif  
