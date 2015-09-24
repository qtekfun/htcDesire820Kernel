// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_RENDERER_PREFERENCES_H_
#define CONTENT_PUBLIC_COMMON_RENDERER_PREFERENCES_H_

#include <string>
#include <vector>

#include "content/common/content_export.h"
#include "third_party/skia/include/core/SkColor.h"

namespace content {

enum RendererPreferencesHintingEnum {
  RENDERER_PREFERENCES_HINTING_SYSTEM_DEFAULT = 0,
  RENDERER_PREFERENCES_HINTING_NONE,
  RENDERER_PREFERENCES_HINTING_SLIGHT,
  RENDERER_PREFERENCES_HINTING_MEDIUM,
  RENDERER_PREFERENCES_HINTING_FULL,
};

enum RendererPreferencesSubpixelRenderingEnum {
  RENDERER_PREFERENCES_SUBPIXEL_RENDERING_SYSTEM_DEFAULT = 0,
  RENDERER_PREFERENCES_SUBPIXEL_RENDERING_NONE,
  RENDERER_PREFERENCES_SUBPIXEL_RENDERING_RGB,
  RENDERER_PREFERENCES_SUBPIXEL_RENDERING_BGR,
  RENDERER_PREFERENCES_SUBPIXEL_RENDERING_VRGB,
  RENDERER_PREFERENCES_SUBPIXEL_RENDERING_VBGR,
};

enum TapMultipleTargetsStrategy {
  TAP_MULTIPLE_TARGETS_STRATEGY_ZOOM = 0,
  TAP_MULTIPLE_TARGETS_STRATEGY_POPUP,
  TAP_MULTIPLE_TARGETS_STRATEGY_NONE,

  TAP_MULTIPLE_TARGETS_STRATEGY_MAX = TAP_MULTIPLE_TARGETS_STRATEGY_NONE,
};

struct CONTENT_EXPORT RendererPreferences {
  RendererPreferences();
  ~RendererPreferences();

  
  
  bool can_accept_load_drops;

  
  
  bool should_antialias_text;

  
  
  RendererPreferencesHintingEnum hinting;

  
  bool use_autohinter;

  
  
  bool use_bitmaps;

  
  
  RendererPreferencesSubpixelRenderingEnum subpixel_rendering;

  
  
  bool use_subpixel_positioning;

  
  SkColor focus_ring_color;

  
  
  SkColor thumb_active_color;
  SkColor thumb_inactive_color;
  SkColor track_color;

  
  SkColor active_selection_bg_color;
  SkColor active_selection_fg_color;
  SkColor inactive_selection_bg_color;
  SkColor inactive_selection_fg_color;

  
  bool browser_handles_non_local_top_level_requests;

  
  bool browser_handles_all_top_level_requests;

  
  bool browser_handles_all_top_level_link_clicks;

  
  
  
  double caret_blink_interval;

  
  bool use_custom_colors;

  
  bool enable_referrers;

  
  bool enable_do_not_track;

  
  double default_zoom_level;

  
  
  std::string user_agent_override;

  
  std::string accept_languages;

  
  
  
  
  
  
  bool report_frame_name_changes;

  
  std::vector<float> touchpad_fling_profile;

  
  std::vector<float> touchscreen_fling_profile;

  
  TapMultipleTargetsStrategy tap_multiple_targets_strategy;

  
  
  bool disable_client_blocked_error_page;

  
  bool plugin_fullscreen_allowed;
};

}  

#endif  
