// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_LOCATION_BAR_PAGE_ACTION_DECORATION_H_
#define CHROME_BROWSER_UI_COCOA_LOCATION_BAR_PAGE_ACTION_DECORATION_H_
#pragma once

#include "chrome/browser/extensions/image_loading_tracker.h"
#import "chrome/browser/ui/cocoa/location_bar/image_decoration.h"
#include "googleurl/src/gurl.h"

class ExtensionAction;
@class ExtensionActionContextMenu;
class LocationBarViewMac;
class Profile;
class TabContents;


class PageActionDecoration : public ImageDecoration,
                             public ImageLoadingTracker::Observer,
                             public NotificationObserver {
 public:
  PageActionDecoration(LocationBarViewMac* owner,
                       Profile* profile,
                       ExtensionAction* page_action);
  virtual ~PageActionDecoration();

  ExtensionAction* page_action() { return page_action_; }
  int current_tab_id() { return current_tab_id_; }
  void set_preview_enabled(bool enabled) { preview_enabled_ = enabled; }
  bool preview_enabled() const { return preview_enabled_; }

  
  virtual void OnImageLoaded(
      SkBitmap* image, const ExtensionResource& resource, int index);

  
  
  
  void UpdateVisibility(TabContents* contents, const GURL& url);

  
  void SetToolTip(NSString* tooltip);
  void SetToolTip(std::string tooltip);

  
  
  NSPoint GetBubblePointInFrame(NSRect frame);

  
  virtual CGFloat GetWidthForSpace(CGFloat width);
  virtual bool AcceptsMousePress();
  virtual bool OnMousePressed(NSRect frame);
  virtual NSString* GetToolTip();
  virtual NSMenu* GetMenu();

 private:
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  LocationBarViewMac* owner_;

  
  Profile* profile_;

  
  
  
  ExtensionAction* page_action_;

  
  
  typedef std::map<std::string, SkBitmap> PageActionMap;
  PageActionMap page_action_icons_;

  
  
  ImageLoadingTracker tracker_;

  
  int current_tab_id_;

  
  GURL current_url_;

  
  scoped_nsobject<NSString> tooltip_;

  
  scoped_nsobject<ExtensionActionContextMenu> menu_;

  
  
  
  bool preview_enabled_;

  
  
  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(PageActionDecoration);
};

#endif  
