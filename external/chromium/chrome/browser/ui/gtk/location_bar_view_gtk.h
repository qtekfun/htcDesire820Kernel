// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_LOCATION_BAR_VIEW_GTK_H_
#define CHROME_BROWSER_UI_GTK_LOCATION_BAR_VIEW_GTK_H_
#pragma once

#include <gtk/gtk.h>

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "chrome/browser/autocomplete/autocomplete_edit.h"
#include "chrome/browser/autocomplete/autocomplete_edit_view_gtk.h"
#include "chrome/browser/extensions/extension_context_menu_model.h"
#include "chrome/browser/extensions/image_loading_tracker.h"
#include "chrome/browser/first_run/first_run.h"
#include "chrome/browser/prefs/pref_member.h"
#include "chrome/browser/ui/gtk/info_bubble_gtk.h"
#include "chrome/browser/ui/gtk/menu_gtk.h"
#include "chrome/browser/ui/gtk/owned_widget_gtk.h"
#include "chrome/browser/ui/omnibox/location_bar.h"
#include "chrome/common/content_settings_types.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "content/common/page_transition_types.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/base/animation/slide_animation.h"
#include "ui/base/gtk/gtk_signal.h"
#include "webkit/glue/window_open_disposition.h"

class AutocompleteEditViewGtk;
class Browser;
class CommandUpdater;
class ContentSettingImageModel;
class ContentSettingBubbleGtk;
class ExtensionAction;
class GtkThemeService;
class Profile;
class SkBitmap;
class TabContents;
class ToolbarModel;

class LocationBarViewGtk : public AutocompleteEditController,
                           public LocationBar,
                           public LocationBarTesting,
                           public NotificationObserver {
 public:
  explicit LocationBarViewGtk(Browser* browser);
  virtual ~LocationBarViewGtk();

  void Init(bool popup_window_mode);

  void SetProfile(Profile* profile);

  
  GtkWidget* widget() { return hbox_.get(); }

  
  GtkWidget* location_icon_widget() const { return location_icon_image_; }

  
  GtkWidget* location_entry_widget() const { return entry_box_; }

  
  TabContents* GetTabContents() const;

  
  
  
  
  
  void SetPreviewEnabledPageAction(ExtensionAction *page_action,
                                   bool preview_enabled);

  
  
  GtkWidget* GetPageActionWidget(ExtensionAction* page_action);

  
  
  
  void Update(const TabContents* tab_for_state_restoring);

  
  void ShowStarBubble(const GURL& url, bool newly_boomkarked);

  
  void SetStarred(bool starred);

  
  virtual void OnAutocompleteAccept(const GURL& url,
                                    WindowOpenDisposition disposition,
                                    PageTransition::Type transition,
                                    const GURL& alternate_nav_url) OVERRIDE;
  virtual void OnChanged() OVERRIDE;
  virtual void OnSelectionBoundsChanged() OVERRIDE;
  virtual void OnKillFocus() OVERRIDE;
  virtual void OnSetFocus() OVERRIDE;
  virtual void OnInputInProgress(bool in_progress) OVERRIDE;
  virtual SkBitmap GetFavicon() const OVERRIDE;
  virtual string16 GetTitle() const OVERRIDE;
  virtual InstantController* GetInstant() OVERRIDE;
  virtual TabContentsWrapper* GetTabContentsWrapper() const OVERRIDE;

  
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

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  static const GdkColor kBackgroundColor;

 private:
  class ContentSettingImageViewGtk : public InfoBubbleGtkDelegate,
                                     public ui::AnimationDelegate {
   public:
    ContentSettingImageViewGtk(ContentSettingsType content_type,
                               const LocationBarViewGtk* parent,
                               Profile* profile);
    virtual ~ContentSettingImageViewGtk();

    GtkWidget* widget() { return alignment_.get(); }

    void set_profile(Profile* profile) { profile_ = profile; }

    bool IsVisible() { return GTK_WIDGET_VISIBLE(widget()); }
    void UpdateFromTabContents(TabContents* tab_contents);

    
    virtual void AnimationProgressed(const ui::Animation* animation);
    virtual void AnimationEnded(const ui::Animation* animation);
    virtual void AnimationCanceled(const ui::Animation* animation);

   private:
    
    void StartAnimating();

    
    void CloseAnimation();

    CHROMEGTK_CALLBACK_1(ContentSettingImageViewGtk, gboolean, OnButtonPressed,
                         GdkEvent*);
    CHROMEGTK_CALLBACK_1(ContentSettingImageViewGtk, gboolean, OnExpose,
                         GdkEventExpose*);

    
    virtual void InfoBubbleClosing(InfoBubbleGtk* info_bubble,
                                   bool closed_by_escape);

    scoped_ptr<ContentSettingImageModel> content_setting_image_model_;

    
    OwnedWidgetGtk alignment_;
    OwnedWidgetGtk event_box_;
    GtkWidget* hbox_;
    OwnedWidgetGtk image_;

    
    OwnedWidgetGtk label_;

    
    const LocationBarViewGtk* parent_;

    
    Profile* profile_;

    
    ContentSettingBubbleGtk* info_bubble_;

    
    ui::SlideAnimation animation_;

    
    GtkRequisition label_req_;

    ScopedRunnableMethodFactory<ContentSettingImageViewGtk> method_factory_;

    DISALLOW_COPY_AND_ASSIGN(ContentSettingImageViewGtk);
  };

  class PageActionViewGtk : public ImageLoadingTracker::Observer,
                            public ExtensionContextMenuModel::PopupDelegate {
   public:
    PageActionViewGtk(
        LocationBarViewGtk* owner, Profile* profile,
        ExtensionAction* page_action);
    virtual ~PageActionViewGtk();

    GtkWidget* widget() { return event_box_.get(); }

    ExtensionAction* page_action() { return page_action_; }

    void set_preview_enabled(bool preview_enabled) {
      preview_enabled_ = preview_enabled;
    }

    bool IsVisible() { return GTK_WIDGET_VISIBLE(widget()); }

    
    
    
    void UpdateVisibility(TabContents* contents, const GURL& url);

    
    virtual void OnImageLoaded(
        SkBitmap* image, const ExtensionResource& resource, int index);

    
    void TestActivatePageAction();

    
    virtual void InspectPopup(ExtensionAction* action);

   private:
    
    
    bool ShowPopup(bool devtools);

    CHROMEGTK_CALLBACK_1(PageActionViewGtk, gboolean, OnButtonPressed,
                         GdkEventButton*);
    CHROMEGTK_CALLBACK_1(PageActionViewGtk, gboolean, OnExposeEvent,
                         GdkEventExpose*);

    
    LocationBarViewGtk* owner_;

    
    Profile* profile_;

    
    
    ExtensionAction* page_action_;

    
    typedef std::map<std::string, GdkPixbuf*> PixbufMap;
    PixbufMap pixbufs_;

    
    
    
    SkBitmap last_icon_skbitmap_;
    GdkPixbuf* last_icon_pixbuf_;

    
    
    ImageLoadingTracker tracker_;

    
    OwnedWidgetGtk event_box_;
    OwnedWidgetGtk image_;

    
    int current_tab_id_;

    
    GURL current_url_;

    
    
    bool preview_enabled_;

    
    scoped_ptr<MenuGtk> context_menu_;
    scoped_refptr<ExtensionContextMenuModel> context_menu_model_;

    DISALLOW_COPY_AND_ASSIGN(PageActionViewGtk);
  };
  friend class PageActionViewGtk;

  
  
  void BuildSiteTypeArea();

  
  
  void SetSiteTypeDragSource();

  GtkWidget* site_type_area() { return site_type_alignment_; }

  CHROMEGTK_CALLBACK_1(LocationBarViewGtk, gboolean, HandleExpose,
                       GdkEventExpose*);
  CHROMEGTK_CALLBACK_1(LocationBarViewGtk, gboolean, OnIconReleased,
                       GdkEventButton*);
  CHROMEGTK_CALLBACK_4(LocationBarViewGtk, void, OnIconDragData,
                       GdkDragContext*, GtkSelectionData*, guint, guint);
  CHROMEGTK_CALLBACK_1(LocationBarViewGtk, void, OnIconDragBegin,
                       GdkDragContext*);
  CHROMEGTK_CALLBACK_1(LocationBarViewGtk, void, OnIconDragEnd,
                       GdkDragContext*);
  CHROMEGTK_CALLBACK_1(LocationBarViewGtk, void, OnHboxSizeAllocate,
                       GtkAllocation*);
  CHROMEGTK_CALLBACK_1(LocationBarViewGtk, void, OnEntryBoxSizeAllocate,
                       GtkAllocation*);
  CHROMEGTK_CALLBACK_1(LocationBarViewGtk, gboolean, OnStarButtonPress,
                       GdkEventButton*);

  
  
  void UpdateSiteTypeArea();

  
  void UpdateEVCertificateLabelSize();

  
  
  
  void SetInfoText();

  
  void SetKeywordLabel(const string16& keyword);

  
  void SetKeywordHintLabel(const string16& keyword);

  void ShowFirstRunBubbleInternal(FirstRun::BubbleType bubble_type);

  
  
  
  void AdjustChildrenVisibility();

  
  void CreateStarButton();

  
  void UpdateStarIcon();

  
  
  bool ShouldOnlyShowLocation();

  
  OwnedWidgetGtk hbox_;

  
  OwnedWidgetGtk star_;
  GtkWidget* star_image_;
  bool starred_;

  
  GtkWidget* site_type_alignment_;
  GtkWidget* site_type_event_box_;
  GtkWidget* location_icon_image_;
  GtkWidget* drag_icon_;
  bool enable_location_drag_;
  
  
  GtkWidget* security_info_label_;

  
  OwnedWidgetGtk content_setting_hbox_;
  ScopedVector<ContentSettingImageViewGtk> content_setting_views_;

  
  OwnedWidgetGtk page_action_hbox_;
  ScopedVector<PageActionViewGtk> page_action_views_;

  
  GtkWidget* entry_box_;

  
  GtkWidget* tab_to_search_alignment_;
  GtkWidget* tab_to_search_box_;
  GtkWidget* tab_to_search_magnifier_;
  GtkWidget* tab_to_search_full_label_;
  GtkWidget* tab_to_search_partial_label_;

  
  GtkWidget* tab_to_search_hint_;
  GtkWidget* tab_to_search_hint_leading_label_;
  GtkWidget* tab_to_search_hint_icon_;
  GtkWidget* tab_to_search_hint_trailing_label_;

  scoped_ptr<AutocompleteEditViewGtk> location_entry_;

  
  GtkWidget* location_entry_alignment_;

  Profile* profile_;
  CommandUpdater* command_updater_;
  ToolbarModel* toolbar_model_;
  Browser* browser_;

  
  
  
  std::wstring location_input_;

  
  WindowOpenDisposition disposition_;

  
  PageTransition::Type transition_;

  
  ScopedRunnableMethodFactory<LocationBarViewGtk> first_run_bubble_;

  
  
  bool popup_window_mode_;

  
  GtkThemeService* theme_service_;

  NotificationRegistrar registrar_;

  
  int hbox_width_;

  
  
  int entry_box_width_;

  
  bool show_selected_keyword_;

  
  bool show_keyword_hint_;

  
  string16 last_keyword_;

  
  BooleanPrefMember edit_bookmarks_enabled_;

  DISALLOW_COPY_AND_ASSIGN(LocationBarViewGtk);
};

#endif  
