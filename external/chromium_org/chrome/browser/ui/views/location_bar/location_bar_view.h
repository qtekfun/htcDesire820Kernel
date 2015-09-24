// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_LOCATION_BAR_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCATION_BAR_LOCATION_BAR_VIEW_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_member.h"
#include "chrome/browser/extensions/extension_context_menu_model.h"
#include "chrome/browser/search_engines/template_url_service_observer.h"
#include "chrome/browser/ui/omnibox/location_bar.h"
#include "chrome/browser/ui/omnibox/omnibox_edit_controller.h"
#include "chrome/browser/ui/search/search_model_observer.h"
#include "chrome/browser/ui/toolbar/toolbar_model.h"
#include "chrome/browser/ui/views/dropdown_bar_host.h"
#include "chrome/browser/ui/views/dropdown_bar_host_delegate.h"
#include "chrome/browser/ui/views/extensions/extension_popup.h"
#include "chrome/browser/ui/views/omnibox/omnibox_view_views.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/gfx/font.h"
#include "ui/gfx/rect.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/drag_controller.h"

class ActionBoxButtonView;
class CommandUpdater;
class ContentSettingBubbleModelDelegate;
class ContentSettingImageView;
class EVBubbleView;
class ExtensionAction;
class GURL;
class GeneratedCreditCardView;
class InstantController;
class KeywordHintView;
class LocationIconView;
class OpenPDFInReaderView;
class ManagePasswordsIconView;
class PageActionWithBadgeView;
class PageActionImageView;
class Profile;
class ScriptBubbleIconView;
class SelectedKeywordView;
class SiteChipView;
class StarView;
class TemplateURLService;
class TranslateIconView;
class ZoomView;

namespace content {
struct SSLStatus;
}

namespace views {
class BubbleDelegateView;
class ImageButton;
class Label;
class LabelButton;
class Widget;
}

class LocationBarView : public LocationBar,
                        public LocationBarTesting,
                        public views::View,
                        public views::ButtonListener,
                        public views::DragController,
                        public OmniboxEditController,
                        public DropdownBarHostDelegate,
                        public TemplateURLServiceObserver,
                        public content::NotificationObserver,
                        public SearchModelObserver {
 public:
  
  static const char kViewClassName[];

  
  virtual void SetFocusAndSelection(bool select_all) OVERRIDE;
  virtual void SetAnimationOffset(int offset) OVERRIDE;

  
  int animation_offset() const { return animation_offset_; }

  class Delegate {
   public:
    
    virtual content::WebContents* GetWebContents() = 0;

    
    virtual InstantController* GetInstant() = 0;

    virtual ToolbarModel* GetToolbarModel() = 0;
    virtual const ToolbarModel* GetToolbarModel() const = 0;

    
    virtual views::Widget* CreateViewsBubble(
        views::BubbleDelegateView* bubble_delegate) = 0;

    
    virtual PageActionImageView* CreatePageActionImageView(
        LocationBarView* owner,
        ExtensionAction* action) = 0;

    
    virtual ContentSettingBubbleModelDelegate*
        GetContentSettingBubbleModelDelegate() = 0;

    
    virtual void ShowWebsiteSettings(content::WebContents* web_contents,
                                     const GURL& url,
                                     const content::SSLStatus& ssl) = 0;

   protected:
    virtual ~Delegate() {}
  };

  enum ColorKind {
    BACKGROUND = 0,
    TEXT,
    SELECTED_TEXT,
    DEEMPHASIZED_TEXT,
    SECURITY_TEXT,
  };

  LocationBarView(Browser* browser,
                  Profile* profile,
                  CommandUpdater* command_updater,
                  Delegate* delegate,
                  bool is_popup_mode);

  virtual ~LocationBarView();

  
  
  
  
  static void InitTouchableLocationBarChildView(views::View* view);

  
  void Init();

  
  
  bool IsInitialized() const;

  
  
  SkColor GetColor(ToolbarModel::SecurityLevel security_level,
                   ColorKind kind) const;

  
  Profile* profile() const { return profile_; }

  
  Delegate* delegate() const { return delegate_; }

  
  void ZoomChangedForActiveTab(bool can_show_bubble);

  
  ZoomView* zoom_view() { return zoom_view_; }

  
  ManagePasswordsIconView* manage_passwords_icon_view() {
    return manage_passwords_icon_view_;
  }

  
  
  
  
  
  void SetPreviewEnabledPageAction(ExtensionAction* page_action,
                                   bool preview_enabled);

  
  views::View* GetPageActionView(ExtensionAction* page_action);

  
  void SetStarToggled(bool on);

  
  StarView* star_view() { return star_view_; }

  
  void SetTranslateIconToggled(bool on);

  
  TranslateIconView* translate_icon_view() { return translate_icon_view_; }

  void set_site_chip_view(SiteChipView* site_chip_view) {
    site_chip_view_ = site_chip_view;
  }

  
  void ShowBookmarkPrompt();

  
  
  gfx::Point GetOmniboxViewOrigin() const;

  
  
  
  void SetImeInlineAutocompletion(const base::string16& text);

  
  void SetGrayTextAutocompletion(const base::string16& text);

  
  base::string16 GetGrayTextAutocompletion() const;

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;

  
  virtual void Layout() OVERRIDE;
  virtual void PaintChildren(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  
  
  
  virtual void SetShowFocusRect(bool show);

  
  
  virtual void SelectAll();

  views::ImageView* GetLocationIconView();
  const views::ImageView* GetLocationIconView() const;

  
  views::View* GetLocationBarAnchor();
  
  
  gfx::Point GetLocationBarAnchorPoint() const;

  OmniboxViewViews* omnibox_view() { return omnibox_view_; }

  views::View* generated_credit_card_view();

  
  virtual void Update(const content::WebContents* contents) OVERRIDE;
  virtual void OnChanged() OVERRIDE;
  virtual void OnSetFocus() OVERRIDE;
  virtual InstantController* GetInstant() OVERRIDE;
  virtual content::WebContents* GetWebContents() OVERRIDE;
  virtual ToolbarModel* GetToolbarModel() OVERRIDE;
  virtual const ToolbarModel* GetToolbarModel() const OVERRIDE;

  
  virtual const char* GetClassName() const OVERRIDE;
  virtual bool HasFocus() const OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds) OVERRIDE;

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  virtual void WriteDragDataForView(View* sender,
                                    const gfx::Point& press_pt,
                                    OSExchangeData* data) OVERRIDE;
  virtual int GetDragOperationsForView(View* sender,
                                       const gfx::Point& p) OVERRIDE;
  virtual bool CanStartDragForView(View* sender,
                                   const gfx::Point& press_pt,
                                   const gfx::Point& p) OVERRIDE;

  
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

  
  virtual void OnTemplateURLServiceChanged() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void ModelChanged(const SearchModel::State& old_state,
                            const SearchModel::State& new_state) OVERRIDE;

  
  
  
  
  int GetInternalHeight(bool use_preferred_size);

  
  
  
  void GetOmniboxPopupPositioningInfo(gfx::Point* top_left_screen_coord,
                                      int* popup_width,
                                      int* left_margin,
                                      int* right_margin);

  
  
  static int GetItemPadding();

  
  static const int kNormalEdgeThickness;
  
  static const int kPopupEdgeThickness;
  
  static const int kIconInternalPadding;
  
  static const int kBubblePadding;

 protected:
  virtual void OnFocus() OVERRIDE;

 private:
  typedef std::vector<ContentSettingImageView*> ContentSettingViews;

  friend class PageActionImageView;
  friend class PageActionWithBadgeView;
  typedef std::vector<ExtensionAction*> PageActions;
  typedef std::vector<PageActionWithBadgeView*> PageActionViews;

  
  
  
  static int GetBuiltInHorizontalPaddingForChildViews();

  
  int GetHorizontalEdgeThickness() const;

  
  int vertical_edge_thickness() const {
    return is_popup_mode_ ? kPopupEdgeThickness : kNormalEdgeThickness;
  }

  
  
  
  bool RefreshContentSettingViews();

  
  void DeletePageActionViews();

  
  
  
  bool RefreshPageActionViews();

  
  size_t ScriptBubbleScriptsRunning();

  
  
  bool RefreshScriptBubble();

  
  
  bool RefreshZoomView();

  
  void RefreshTranslateIcon();

  
  void ToggleVisibility(bool new_vis, views::View* view);

  
  bool RefreshManagePasswordsIconView();

  
  void ShowManagePasswordsBubbleIfNeeded();

  
  bool HasValidSuggestText() const;

  
  void ShowFirstRunBubbleInternal();

  
  
  void PaintPageActionBackgrounds(gfx::Canvas* canvas);

  
  
  
  void AccessibilitySetValue(const base::string16& new_value);

  
  bool IsBookmarkStarHiddenByExtension();

  
  
  
  Browser* browser_;

  OmniboxViewViews* omnibox_view_;

  
  Profile* profile_;

  
  Delegate* delegate_;

  
  scoped_ptr<views::Painter> background_border_painter_;
  scoped_ptr<views::Painter> background_filling_painter_;

  
  LocationIconView* location_icon_view_;

  
  EVBubbleView* ev_bubble_view_;

  
  
  
  
  views::Label* ime_inline_autocomplete_view_;

  
  
  
  
  

  
  SelectedKeywordView* selected_keyword_view_;

  
  
  views::Label* suggested_text_view_;

  
  KeywordHintView* keyword_hint_view_;

  
  views::ImageButton* mic_search_view_;

  
  ContentSettingViews content_setting_views_;

  
  ZoomView* zoom_view_;

  
  GeneratedCreditCardView* generated_credit_card_view_;

  
  OpenPDFInReaderView* open_pdf_in_reader_view_;

  
  ManagePasswordsIconView* manage_passwords_icon_view_;

  
  PageActions page_actions_;

  
  PageActionViews page_action_views_;

  
  ScriptBubbleIconView* script_bubble_icon_view_;

  
  SiteChipView* site_chip_view_;

  
  TranslateIconView* translate_icon_view_;

  
  StarView* star_view_;

  
  views::LabelButton* search_button_;

  
  
  const bool is_popup_mode_;

  
  
  bool show_focus_rect_;

  
  
  TemplateURLService* template_url_service_;

  
  BooleanPrefMember edit_bookmarks_enabled_;

  
  
  
  
  int animation_offset_;

  
  content::NotificationRegistrar registrar_;

  
  base::WeakPtrFactory<LocationBarView> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(LocationBarView);
};

#endif  
