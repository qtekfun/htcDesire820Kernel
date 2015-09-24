// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_LOCATION_BAR_PAGE_ACTION_DECORATION_H_
#define CHROME_BROWSER_UI_COCOA_LOCATION_BAR_PAGE_ACTION_DECORATION_H_

#include "chrome/browser/extensions/extension_action.h"
#include "chrome/browser/extensions/extension_action_icon_factory.h"
#import "chrome/browser/ui/cocoa/location_bar/image_decoration.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "url/gurl.h"

@class ExtensionActionContextMenuController;
class Browser;
class LocationBarViewMac;

namespace content {
class WebContents;
}


class PageActionDecoration : public ImageDecoration,
                             public ExtensionActionIconFactory::Observer,
                             public content::NotificationObserver,
                             public ExtensionAction::IconAnimation::Observer {
 public:
  PageActionDecoration(LocationBarViewMac* owner,
                       Browser* browser,
                       ExtensionAction* page_action);
  virtual ~PageActionDecoration();

  ExtensionAction* page_action() { return page_action_; }
  int current_tab_id() { return current_tab_id_; }
  void set_preview_enabled(bool enabled) { preview_enabled_ = enabled; }
  bool preview_enabled() const { return preview_enabled_; }

  
  virtual void OnIconUpdated() OVERRIDE;

  
  
  
  void UpdateVisibility(content::WebContents* contents, const GURL& url);

  
  void SetToolTip(NSString* tooltip);
  void SetToolTip(std::string tooltip);

  
  
  NSPoint GetBubblePointInFrame(NSRect frame);

  
  virtual CGFloat GetWidthForSpace(CGFloat width) OVERRIDE;
  virtual void DrawWithBackgroundInFrame(NSRect background_frame,
                                         NSRect frame,
                                         NSView* control_view) OVERRIDE;
  virtual bool AcceptsMousePress() OVERRIDE;
  virtual bool OnMousePressed(NSRect frame) OVERRIDE;
  virtual NSString* GetToolTip() OVERRIDE;
  virtual NSMenu* GetMenu() OVERRIDE;

 private:
  
  bool ActivatePageAction(NSRect frame);

  
  void ShowPopup(const NSRect& frame, const GURL& popup_url);

  
  virtual void OnIconChanged() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  LocationBarViewMac* owner_;

  
  Browser* browser_;

  
  
  
  ExtensionAction* page_action_;


  
  
  
  
  scoped_ptr<ExtensionActionIconFactory> icon_factory_;

  
  int current_tab_id_;

  
  GURL current_url_;

  
  base::scoped_nsobject<NSString> tooltip_;

  
  base::scoped_nsobject<
      ExtensionActionContextMenuController> contextMenuController_;

  
  
  
  bool preview_enabled_;

  
  ExtensionAction::IconAnimation::ScopedObserver
      scoped_icon_animation_observer_;

  
  
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(PageActionDecoration);
};

#endif  
