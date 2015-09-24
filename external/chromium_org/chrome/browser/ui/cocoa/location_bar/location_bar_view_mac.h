// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_LOCATION_BAR_LOCATION_BAR_VIEW_MAC_H_
#define CHROME_BROWSER_UI_COCOA_LOCATION_BAR_LOCATION_BAR_VIEW_MAC_H_

#include <string>

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_member.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/cocoa/omnibox/omnibox_view_mac.h"
#include "chrome/browser/ui/omnibox/location_bar.h"
#include "chrome/browser/ui/omnibox/omnibox_edit_controller.h"
#include "chrome/browser/ui/search/search_model_observer.h"
#include "chrome/common/content_settings_types.h"

@class AutocompleteTextField;
class CommandUpdater;
class ContentSettingDecoration;
class EVBubbleDecoration;
class GeneratedCreditCardDecoration;
class KeywordHintDecoration;
class LocationBarDecoration;
class LocationIconDecoration;
class MicSearchDecoration;
class PageActionDecoration;
class Profile;
class SearchButtonDecoration;
class SelectedKeywordDecoration;
class StarDecoration;
class ZoomDecoration;
class ZoomDecorationTest;


class LocationBarViewMac : public LocationBar,
                           public LocationBarTesting,
                           public OmniboxEditController,
                           public content::NotificationObserver,
                           public SearchModelObserver {
 public:
  LocationBarViewMac(AutocompleteTextField* field,
                     CommandUpdater* command_updater,
                     Profile* profile,
                     Browser* browser);
  virtual ~LocationBarViewMac();

  
  virtual void ShowFirstRunBubble() OVERRIDE;
  virtual GURL GetDestinationURL() const OVERRIDE;
  virtual WindowOpenDisposition GetWindowOpenDisposition() const OVERRIDE;
  virtual content::PageTransition GetPageTransition() const OVERRIDE;
  virtual void AcceptInput() OVERRIDE;
  virtual void FocusLocation(bool select_all) OVERRIDE;
  virtual void FocusSearch() OVERRIDE;
  virtual void UpdateContentSettingsIcons() OVERRIDE;
  virtual void UpdateManagePasswordsIconAndBubble() OVERRIDE {};
  virtual void UpdatePageActions() OVERRIDE;
  virtual void InvalidatePageActions() OVERRIDE;
  virtual void UpdateOpenPDFInReaderPrompt() OVERRIDE;
  virtual void UpdateGeneratedCreditCardView() OVERRIDE;
  virtual void SaveStateToContents(content::WebContents* contents) OVERRIDE;
  virtual void Revert() OVERRIDE;
  virtual const OmniboxView* GetOmniboxView() const OVERRIDE;
  virtual OmniboxView* GetOmniboxView() OVERRIDE;
  virtual LocationBarTesting* GetLocationBarForTesting() OVERRIDE;

  
  virtual int PageActionCount() OVERRIDE;
  virtual int PageActionVisibleCount() OVERRIDE;
  virtual ExtensionAction* GetPageAction(size_t index) OVERRIDE;
  virtual ExtensionAction* GetVisiblePageAction(size_t index) OVERRIDE;
  virtual void TestPageActionPressed(size_t index) OVERRIDE;
  virtual bool GetBookmarkStarVisibility() OVERRIDE;

  
  void SetEditable(bool editable);
  bool IsEditable();

  
  void SetStarred(bool starred);

  
  
  
  
  
  void ZoomChangedForActiveTab(bool can_show_bubble);

  
  
  NSPoint GetBookmarkBubblePoint() const;

  
  
  NSPoint GetPageInfoBubblePoint() const;

  
  
  NSPoint GetGeneratedCreditCardBubblePoint() const;

  
  
  void OnDecorationsChanged();

  
  void Layout();

  
  void RedrawDecoration(LocationBarDecoration* decoration);

  
  
  
  
  
  void SetPreviewEnabledPageAction(ExtensionAction* page_action,
                                   bool preview_enabled);

  
  NSRect GetPageActionFrame(ExtensionAction* page_action);

  
  
  
  
  NSPoint GetPageActionBubblePoint(ExtensionAction* page_action);

  
  virtual void Update(const content::WebContents* contents) OVERRIDE;
  virtual void OnChanged() OVERRIDE;
  virtual void OnSetFocus() OVERRIDE;
  virtual InstantController* GetInstant() OVERRIDE;
  virtual content::WebContents* GetWebContents() OVERRIDE;
  virtual ToolbarModel* GetToolbarModel() OVERRIDE;
  virtual const ToolbarModel* GetToolbarModel() const OVERRIDE;

  NSImage* GetKeywordImage(const base::string16& keyword);

  AutocompleteTextField* GetAutocompleteTextField() { return field_; }


  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void ModelChanged(const SearchModel::State& old_state,
                            const SearchModel::State& new_state) OVERRIDE;

  Browser* browser() const { return browser_; }

 private:
  friend ZoomDecorationTest;

  
  void PostNotification(NSString* notification);

  
  PageActionDecoration* GetPageActionDecoration(ExtensionAction* page_action);

  
  void DeletePageActionDecorations();

  void OnEditBookmarksEnabledChanged();

  
  
  void RefreshPageActionDecorations();

  
  
  bool RefreshContentSettingsDecorations();

  void ShowFirstRunBubbleInternal();

  
  bool IsStarEnabled();

  
  void UpdateZoomDecoration();

  
  void UpdateStarDecorationVisibility();

  
  
  bool UpdateMicSearchDecorationVisibility();

  
  bool IsBookmarkStarHiddenByExtension();

  scoped_ptr<OmniboxViewMac> omnibox_view_;

  AutocompleteTextField* field_;  

  
  scoped_ptr<LocationIconDecoration> location_icon_decoration_;

  
  scoped_ptr<SelectedKeywordDecoration> selected_keyword_decoration_;

  
  
  scoped_ptr<EVBubbleDecoration> ev_bubble_decoration_;

  
  scoped_ptr<StarDecoration> star_decoration_;

  
  
  scoped_ptr<ZoomDecoration> zoom_decoration_;

  
  std::vector<ExtensionAction*> page_actions_;

  
  ScopedVector<PageActionDecoration> page_action_decorations_;

  
  ScopedVector<ContentSettingDecoration> content_setting_decorations_;

  
  scoped_ptr<KeywordHintDecoration> keyword_hint_decoration_;

  
  scoped_ptr<MicSearchDecoration> mic_search_decoration_;

  
  scoped_ptr<GeneratedCreditCardDecoration> generated_credit_card_decoration_;

  
  scoped_ptr<SearchButtonDecoration> search_button_decoration_;

  Profile* profile_;

  Browser* browser_;

  
  content::NotificationRegistrar registrar_;

  
  base::WeakPtrFactory<LocationBarViewMac> weak_ptr_factory_;

  
  BooleanPrefMember edit_bookmarks_enabled_;

  DISALLOW_COPY_AND_ASSIGN(LocationBarViewMac);
};

#endif  
