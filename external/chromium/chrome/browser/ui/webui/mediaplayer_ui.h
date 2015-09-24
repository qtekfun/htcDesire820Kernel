// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_MEDIAPLAYER_UI_H_
#define CHROME_BROWSER_UI_WEBUI_MEDIAPLAYER_UI_H_
#pragma once

#include <set>
#include <vector>

#include "chrome/browser/ui/webui/chrome_url_data_manager.h"
#include "content/browser/webui/web_ui.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "content/common/notification_source.h"
#include "content/common/notification_type.h"
#include "net/base/directory_lister.h"
#include "net/url_request/url_request.h"

template <typename T> struct DefaultSingletonTraits;

class Browser;
class GURL;
class MediaplayerHandler;
class Profile;

class MediaPlayer : public NotificationObserver,
                    public net::URLRequest::Interceptor {
 public:
  ~MediaPlayer();

  
  
  void EnqueueMediaFile(Profile* profile, const FilePath& file_path,
                        Browser* creator);

  
  
  void EnqueueMediaFileUrl(const GURL& url, Browser* creator);

  
  
  
  void ForcePlayMediaFile(Profile* profile, const FilePath& file_path,
                          Browser* creator);

  
  
  void ForcePlayMediaURL(const GURL& url, Browser* creator);

  
  void TogglePlaylistWindowVisible();

  
  void ShowPlaylistWindow();

  
  void ToggleFullscreen();

  
  void ClosePlaylistWindow();

  
  void SetPlaylistOffset(int offset);

  
  
  
  void SetNewHandler(MediaplayerHandler* handler,
                     TabContents* contents);

  
  void RemoveHandler(MediaplayerHandler* handler);

  
  
  void RegisterNewPlaylistHandler(MediaplayerHandler* handler,
                                  TabContents* contents);

  
  void RemovePlaylistHandler(MediaplayerHandler* handler);

  
  
  void NotifyPlaylistChanged();

  
  
  
  virtual net::URLRequestJob* MaybeIntercept(net::URLRequest* request);

  
  
  
  
  virtual net::URLRequestJob* MaybeInterceptResponse(net::URLRequest* request);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  static MediaPlayer* GetInstance();

 private:
  friend struct DefaultSingletonTraits<MediaPlayer>;

  MediaPlayer();

  
  
  void PopupMediaPlayer(Browser* creator);

  
  
  void PopupPlaylist(Browser* creator);

  
  void RegisterListeners();

  
  
  MediaplayerHandler* handler_;

  
  
  MediaplayerHandler* playlist_;

  
  
  Browser* playlist_browser_;

  
  
  
  Browser* mediaplayer_browser_;

  
  
  std::vector<GURL> unhandled_urls_;

  
  NotificationRegistrar registrar_;

  
  
  
  TabContents* mediaplayer_tab_;

  
  
  
  TabContents* playlist_tab_;

  
  
  std::set<std::string> supported_mime_types_;
  DISALLOW_COPY_AND_ASSIGN(MediaPlayer);
};

class MediaplayerUI : public WebUI {
 public:
  explicit MediaplayerUI(TabContents* contents);

 private:
  DISALLOW_COPY_AND_ASSIGN(MediaplayerUI);
};

#endif  
