// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_WEB_NAVIGATION_FRAME_NAVIGATION_STATE_H_
#define CHROME_BROWSER_EXTENSIONS_API_WEB_NAVIGATION_FRAME_NAVIGATION_STATE_H_

#include <map>
#include <set>

#include "base/compiler_specific.h"
#include "url/gurl.h"

namespace content {
class RenderViewHost;
}

namespace extensions {

class FrameNavigationState {
 public:
  
  struct FrameID {
    FrameID();
    FrameID(int64 frame_num, content::RenderViewHost* render_view_host);

    bool operator<(const FrameID& other) const;
    bool operator==(const FrameID& other) const;
    bool operator!=(const FrameID& other) const;

    int64 frame_num;
    content::RenderViewHost* render_view_host;
  };
  typedef std::set<FrameID>::const_iterator const_iterator;

  FrameNavigationState();
  ~FrameNavigationState();

  
  const_iterator begin() const { return frame_ids_.begin(); }
  const_iterator end() const { return frame_ids_.end(); }

  
  bool CanSendEvents(FrameID frame_id) const;

  
  bool IsValidUrl(const GURL& url) const;

  
  void TrackFrame(FrameID frame_id,
                  FrameID parent_frame_id,
                  const GURL& url,
                  bool is_main_frame,
                  bool is_error_page,
                  bool is_iframe_srcdoc);

  
  void FrameDetached(FrameID frame_id);

  
  
  void StopTrackingFramesInRVH(content::RenderViewHost* render_view_host,
                               FrameID id_to_skip);

  
  void UpdateFrame(FrameID frame_id, const GURL& url);

  
  bool IsValidFrame(FrameID frame_id) const;

  
  GURL GetUrl(FrameID frame_id) const;

  
  
  bool IsMainFrame(FrameID frame_id) const;

  
  
  FrameID GetMainFrameID() const;

  
  FrameID GetParentFrameID(FrameID frame_id) const;

  
  
  void SetErrorOccurredInFrame(FrameID frame_id);

  
  bool GetErrorOccurredInFrame(FrameID frame_id) const;

  
  
  void SetNavigationCompleted(FrameID frame_id);

  
  bool GetNavigationCompleted(FrameID frame_id) const;

  
  void SetParsingFinished(FrameID frame_id);

  
  bool GetParsingFinished(FrameID frame_id) const;

  
  
  void SetNavigationCommitted(FrameID frame_id);

  
  bool GetNavigationCommitted(FrameID frame_id) const;

  
  void SetIsServerRedirected(FrameID frame_id);

  
  bool GetIsServerRedirected(FrameID frame_id) const;

#ifdef UNIT_TEST
  static void set_allow_extension_scheme(bool allow_extension_scheme) {
    allow_extension_scheme_ = allow_extension_scheme;
  }
#endif

 private:
  struct FrameState {
    FrameState();

    bool error_occurred;  
    bool is_main_frame;  
    bool is_iframe_srcdoc;  
    bool is_navigating;  
    bool is_committed;  
    bool is_server_redirected;  
    bool is_parsing;  
    int64 parent_frame_num;
    GURL url;  
  };
  typedef std::map<FrameID, FrameState> FrameIdToStateMap;

  
  FrameIdToStateMap frame_state_map_;

  
  std::set<FrameID> frame_ids_;

  
  FrameID main_frame_id_;

  
  static bool allow_extension_scheme_;

  DISALLOW_COPY_AND_ASSIGN(FrameNavigationState);
};

}  

#endif  
