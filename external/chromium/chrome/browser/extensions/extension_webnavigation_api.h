// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_WEBNAVIGATION_API_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_WEBNAVIGATION_API_H_
#pragma once

#include <map>

#include "base/memory/singleton.h"
#include "chrome/browser/extensions/extension_function.h"
#include "content/browser/tab_contents/tab_contents_observer.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "googleurl/src/gurl.h"

class TabContents;
struct ViewHostMsg_CreateWindow_Params;

class FrameNavigationState {
 public:
  FrameNavigationState();
  ~FrameNavigationState();

  
  bool CanSendEvents(int64 frame_id) const;

  
  
  void TrackFrame(int64 frame_id,
                  const GURL& url,
                  bool is_main_frame,
                  bool is_error_page,
                  const TabContents* tab_contents);

  
  GURL GetUrl(int64 frame_id) const;

  
  bool IsMainFrame(int64 frame_id) const;

  
  void ErrorOccurredInFrame(int64 frame_id);

  
  void RemoveTabContentsState(const TabContents* tab_contents);

#ifdef UNIT_TEST
  static void set_allow_extension_scheme(bool allow_extension_scheme) {
    allow_extension_scheme_ = allow_extension_scheme;
  }
#endif

 private:
  typedef std::multimap<const TabContents*, int64> TabContentsToFrameIdMap;
  struct FrameState {
    bool error_occurred;  
    bool is_main_frame;  
    GURL url;  
  };
  typedef std::map<int64, FrameState> FrameIdToStateMap;

  
  TabContentsToFrameIdMap tab_contents_map_;

  
  FrameIdToStateMap frame_state_map_;

  
  static bool allow_extension_scheme_;

  DISALLOW_COPY_AND_ASSIGN(FrameNavigationState);
};

class ExtensionWebNavigationTabObserver : public TabContentsObserver {
 public:
  explicit ExtensionWebNavigationTabObserver(TabContents* tab_contents);
  virtual ~ExtensionWebNavigationTabObserver();

  
  virtual void DidStartProvisionalLoadForFrame(int64 frame_id,
                                                 bool is_main_frame,
                                                 const GURL& validated_url,
                                                 bool is_error_page) OVERRIDE;
  virtual void DidCommitProvisionalLoadForFrame(
      int64 frame_id,
      bool is_main_frame,
      const GURL& url,
      PageTransition::Type transition_type) OVERRIDE;
  virtual void DidFailProvisionalLoad(int64 frame_id,
                                      bool is_main_frame,
                                      const GURL& validated_url,
                                      int error_code) OVERRIDE;
  virtual void DocumentLoadedInFrame(int64 frame_id) OVERRIDE;
  virtual void DidFinishLoad(int64 frame_id) OVERRIDE;
  virtual void TabContentsDestroyed(TabContents* tab) OVERRIDE;
  virtual void DidOpenURL(const GURL& url,
                          const GURL& referrer,
                          WindowOpenDisposition disposition,
                          PageTransition::Type transition);


 private:
  
  
  bool IsReferenceFragmentNavigation(int64 frame_id, const GURL& url);

  
  void NavigatedReferenceFragment(int64 frame_id,
                                  bool is_main_frame,
                                  const GURL& url,
                                  PageTransition::Type transition_type);

  
  FrameNavigationState navigation_state_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionWebNavigationTabObserver);
};

class ExtensionWebNavigationEventRouter : public NotificationObserver {
 public:
  
  static ExtensionWebNavigationEventRouter* GetInstance();

  
  
  void Init();

 private:
  friend struct DefaultSingletonTraits<ExtensionWebNavigationEventRouter>;

  ExtensionWebNavigationEventRouter();
  virtual ~ExtensionWebNavigationEventRouter();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  
  void CreatingNewWindow(TabContents* tab_content,
                         const ViewHostMsg_CreateWindow_Params* details);

  
  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionWebNavigationEventRouter);
};

#endif  
