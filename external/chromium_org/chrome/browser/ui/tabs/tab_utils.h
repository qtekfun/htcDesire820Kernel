// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TABS_TAB_UTILS_H_
#define CHROME_BROWSER_UI_TABS_TAB_UTILS_H_

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"

namespace content {
class WebContents;
}  

namespace gfx {
class Animation;
class Image;
}  

enum TabMediaState {
  TAB_MEDIA_STATE_NONE,
  TAB_MEDIA_STATE_RECORDING,  
  TAB_MEDIA_STATE_CAPTURING,  
  TAB_MEDIA_STATE_AUDIO_PLAYING  
};

namespace chrome {

bool ShouldTabShowFavicon(int capacity,
                          bool is_pinned_tab,
                          bool is_active_tab,
                          bool has_favicon,
                          TabMediaState media_state);
bool ShouldTabShowMediaIndicator(int capacity,
                                 bool is_pinned_tab,
                                 bool is_active_tab,
                                 bool has_favicon,
                                 TabMediaState media_state);
bool ShouldTabShowCloseButton(int capacity,
                              bool is_pinned_tab,
                              bool is_active_tab);

bool IsPlayingAudio(content::WebContents* contents);

TabMediaState GetTabMediaStateForContents(content::WebContents* contents);

const gfx::Image& GetTabMediaIndicatorImage(TabMediaState media_state);

scoped_ptr<gfx::Animation> CreateTabMediaIndicatorFadeAnimation(
    TabMediaState next_media_state);

base::string16 AssembleTabTooltipText(const base::string16& title,
                                      TabMediaState media_state);

}  

#endif  
