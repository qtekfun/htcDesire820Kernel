// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_LOCATION_BAR_VIEW_GTK_H_
#define CHROME_BROWSER_UI_GTK_LOCATION_BAR_VIEW_GTK_H_

#include <gtk/gtk.h>

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_member.h"
#include "chrome/browser/extensions/extension_action.h"
#include "chrome/browser/extensions/extension_action_icon_factory.h"
#include "chrome/browser/extensions/extension_context_menu_model.h"
#include "chrome/browser/ui/gtk/bubble/bubble_gtk.h"
#include "chrome/browser/ui/gtk/menu_gtk.h"
#include "chrome/browser/ui/omnibox/location_bar.h"
#include "chrome/browser/ui/omnibox/omnibox_edit_controller.h"
#include "chrome/browser/ui/view_ids.h"
#include "chrome/common/content_settings_types.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/common/page_transition_types.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/owned_widget_gtk.h"
#include "ui/base/window_open_disposition.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/animation/slide_animation.h"
#include "url/gurl.h"

class Browser;
class CommandUpdater;
class ContentSettingImageModel;
class ContentSettingBubbleGtk;
class ExtensionAction;
class GtkThemeService;
class OmniboxViewGtk;

namespace content {
class WebContents;
}

namespace gfx {
class Image;
}

namespace ui {
class Accelerator;
}

class LocationBarViewGtk : public OmniboxEditController,
                           public LocationBar,
                           public LocationBarTesting,
                           public content::NotificationObserver {
 public:
  explicit LocationBarViewGtk(Browser* browser);
  virtual ~LocationBarViewGtk();

  void Init(bool popup_window_mode);

  
  GtkWidget* widget() { return hbox_.get(); }

  
  GtkWidget* location_icon_widget() const { return location_icon_image_; }

  
  GtkWidget* location_entry_widget() const { return entry_box_; }

  Browser* browser() const { return browser_; }

  
  
  
  
  
  void SetPreviewEnabledPageAction(ExtensionAction *page_action,
                                   bool preview_enabled);

  
  
  GtkWidget* GetPageActionWidget(ExtensionAction* page_action);

  
  void ShowStarBubble(const GURL& url, bool newly_boomkarked);

  
  
  void ZoomChangedForActiveTab(bool can_show_bubble);

  
  GtkWidget* zoom_widget() { return zoom_.get(); }

  
  
  GtkWidget* manage_passwords_icon_widget() {
    return manage_passwords_icon_.get();
  }

  
  void SetStarred(bool starred);

  
  virtual void Update(const content::WebContents* contents) OVERRIDE;
  virtual void OnChanged() OVERRIDE;
  virtual void OnSetFocus() OVERRIDE;
  virtual InstantController* GetInstant() OVERRIDE;
  virtual content::WebContents* GetWebContents() OVERRIDE;
  virtual ToolbarModel* GetToolbarModel() OVERRIDE;
  virtual const ToolbarModel* GetToolbarModel() const OVERRIDE;

  
  virtual void ShowFirstRunBubble() OVERRIDE;
  virtual GURL GetDestinationURL() const OVERRIDE;
  virtual WindowOpenDisposition GetWindowOpenDisposition() const OVERRIDE;
  virtual content::PageTransition GetPageTransition() const OVERRIDE;
  virtual void AcceptInput() OVERRIDE;
  virtual void FocusLocation(bool select_all) OVERRIDE;
  virtual void FocusSearch() OVERRIDE;
  virtual void UpdateContentSettingsIcons() OVERRIDE;
  virtual void UpdateManagePasswordsIconAndBubble() OVERRIDE;
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

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  static const GdkColor kBackgroundColor;

  
  
  class PageToolViewGtk : public gfx::AnimationDelegate {
   public:
    PageToolViewGtk();
    virtual ~PageToolViewGtk();

    GtkWidget* widget() { return alignment_.get(); }

    bool IsVisible();

    virtual void Update(content::WebContents* web_contents) = 0;

    
    virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;
    virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;
    virtual void AnimationCanceled(const gfx::Animation* animation) OVERRIDE;

   protected:
    
    virtual GdkColor GetButtonBorderColor() const = 0;
    virtual GdkColor GetGradientTopColor() const = 0;
    virtual GdkColor GetGradientBottomColor() const = 0;

    
    virtual void OnClick(GtkWidget* sender) = 0;

    
    void StartAnimating();

    
    void CloseAnimation();

    CHROMEGTK_CALLBACK_1(PageToolViewGtk, gboolean, OnButtonPressed, GdkEvent*);
    CHROMEGTK_CALLBACK_1(PageToolViewGtk, gboolean, OnExpose, GdkEventExpose*);

    
    ui::OwnedWidgetGtk alignment_;
    ui::OwnedWidgetGtk event_box_;
    GtkWidget* hbox_;
    ui::OwnedWidgetGtk image_;

    
    ui::OwnedWidgetGtk label_;

    
    gfx::SlideAnimation animation_;

    
    GtkRequisition label_req_;

    base::WeakPtrFactory<PageToolViewGtk> weak_factory_;

   private:
    DISALLOW_COPY_AND_ASSIGN(PageToolViewGtk);
  };

 private:
  class PageActionViewGtk :
       public ExtensionActionIconFactory::Observer,
       public content::NotificationObserver,
       public ExtensionContextMenuModel::PopupDelegate,
       public ExtensionAction::IconAnimation::Observer {
   public:
    PageActionViewGtk(LocationBarViewGtk* owner, ExtensionAction* page_action);
    virtual ~PageActionViewGtk();

    GtkWidget* widget() { return event_box_.get(); }

    ExtensionAction* page_action() { return page_action_; }

    void set_preview_enabled(bool preview_enabled) {
      preview_enabled_ = preview_enabled;
    }

    bool IsVisible();

    
    
    
    void UpdateVisibility(content::WebContents* contents, const GURL& url);

    
    virtual void OnIconUpdated() OVERRIDE;

    
    void TestActivatePageAction();

    
    virtual void Observe(int type,
                         const content::NotificationSource& source,
                         const content::NotificationDetails& details) OVERRIDE;

    
    virtual void InspectPopup(ExtensionAction* action) OVERRIDE;

   private:
    
    void ConnectPageActionAccelerator();

    
    void DisconnectPageActionAccelerator();

    CHROMEGTK_CALLBACK_1(PageActionViewGtk, gboolean, OnButtonPressed,
                         GdkEventButton*);
    CHROMEGTK_CALLBACK_1(PageActionViewGtk, gboolean, OnExposeEvent,
                         GdkEventExpose*);
    CHROMEGTK_CALLBACK_0(PageActionViewGtk, void, OnRealize);

    
    
    static gboolean OnGtkAccelerator(GtkAccelGroup* accel_group,
                                     GObject* acceleratable,
                                     guint keyval,
                                     GdkModifierType modifier,
                                     void* user_data);

    
    virtual void OnIconChanged() OVERRIDE;

    
    LocationBarViewGtk* owner_;

    
    
    ExtensionAction* page_action_;

    
    
    
    
    scoped_ptr<ExtensionActionIconFactory> icon_factory_;

    
    ui::OwnedWidgetGtk event_box_;
    ui::OwnedWidgetGtk image_;

    
    int current_tab_id_;

    
    GURL current_url_;

    
    gfx::NativeWindow window_;

    
    content::NotificationRegistrar registrar_;

    
    GtkAccelGroup* accel_group_;

    
    scoped_ptr<ui::Accelerator> page_action_keybinding_;
    
    scoped_ptr<ui::Accelerator> script_badge_keybinding_;

    
    
    bool preview_enabled_;

    
    scoped_ptr<MenuGtk> context_menu_;
    scoped_refptr<ExtensionContextMenuModel> context_menu_model_;

    
    ExtensionAction::IconAnimation::ScopedObserver
        scoped_icon_animation_observer_;

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
  CHROMEGTK_CALLBACK_1(LocationBarViewGtk, gboolean, OnZoomButtonPress,
                       GdkEventButton*);
  CHROMEGTK_CALLBACK_1(LocationBarViewGtk, gboolean,
                       OnManagePasswordsIconButtonPress, GdkEventButton*);
  CHROMEGTK_CALLBACK_1(LocationBarViewGtk, gboolean, OnScriptBubbleButtonPress,
                       GdkEventButton*);
  CHROMEGTK_CALLBACK_1(LocationBarViewGtk, void, OnStarButtonSizeAllocate,
                       GtkAllocation*);
  CHROMEGTK_CALLBACK_1(LocationBarViewGtk, gboolean, OnStarButtonPress,
                       GdkEventButton*);
  CHROMEGTK_CALLBACK_1(LocationBarViewGtk, gboolean,
                       OnScriptBubbleButtonExpose, GdkEventExpose*);

  
  
  void UpdateSiteTypeArea();

  
  void UpdateEVCertificateLabelSize();

  
  void SetKeywordLabel(const base::string16& keyword);

  
  void SetKeywordHintLabel(const base::string16& keyword);

  void ShowFirstRunBubbleInternal();

  
  void ShowZoomBubble();

  
  void ShowManagePasswordsBubble();

  
  
  
  void AdjustChildrenVisibility();

  
  
  GtkWidget* CreateIconButton(
      GtkWidget** image,
      int image_id,
      ViewID debug_id,
      int tooltip_id,
      gboolean (click_callback)(GtkWidget*, GdkEventButton*, gpointer));
  void CreateZoomButton();
  void CreateManagePasswordsIconButton();
  void CreateScriptBubbleButton();
  void CreateStarButton();

  
  
  void UpdateZoomIcon();
  void UpdateManagePasswordsIcon();
  void UpdateScriptBubbleIcon();
  void UpdateStarIcon();

  
  void ShowManagePasswordsBubbleIfNeeded();

  
  
  bool ShouldOnlyShowLocation();

  
  ui::OwnedWidgetGtk hbox_;

  
  ui::OwnedWidgetGtk zoom_;
  GtkWidget* zoom_image_;

  
  ui::OwnedWidgetGtk manage_passwords_icon_;
  GtkWidget* manage_passwords_icon_image_;

  ui::OwnedWidgetGtk script_bubble_button_;
  GtkWidget* script_bubble_button_image_;
  size_t num_running_scripts_;

  
  ui::OwnedWidgetGtk star_;
  GtkWidget* star_image_;
  bool starred_;
  bool star_sized_;  

  
  
  base::Closure on_star_sized_;

  
  GtkWidget* site_type_alignment_;
  GtkWidget* site_type_event_box_;
  GtkWidget* location_icon_image_;
  GtkWidget* drag_icon_;
  bool enable_location_drag_;
  
  
  GtkWidget* security_info_label_;

  
  ui::OwnedWidgetGtk content_setting_hbox_;
  ScopedVector<PageToolViewGtk> content_setting_views_;

  
  std::vector<ExtensionAction*> page_actions_;

  
  ui::OwnedWidgetGtk page_action_hbox_;
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

  scoped_ptr<OmniboxViewGtk> omnibox_view_;

  
  GtkWidget* omnibox_view_alignment_;

  Browser* browser_;

  
  
  bool popup_window_mode_;

  
  GtkThemeService* theme_service_;

  content::NotificationRegistrar registrar_;

  
  int hbox_width_;

  
  
  int entry_box_width_;

  
  bool show_selected_keyword_;

  
  bool show_keyword_hint_;

  
  base::string16 last_keyword_;

  
  BooleanPrefMember edit_bookmarks_enabled_;

  
  GURL drag_url_;
  base::string16 drag_title_;

  
  base::WeakPtrFactory<LocationBarViewGtk> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(LocationBarViewGtk);
};

#endif  
