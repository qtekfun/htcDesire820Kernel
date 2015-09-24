// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "base/memory/shared_memory.h"
#include "base/process/process.h"
#include "base/strings/string16.h"
#include "cc/output/begin_frame_args.h"
#include "cc/output/compositor_frame.h"
#include "cc/output/compositor_frame_ack.h"
#include "content/common/browser_rendering_stats.h"
#include "content/common/content_export.h"
#include "content/common/content_param_traits.h"
#include "content/common/cookie_data.h"
#include "content/common/navigation_gesture.h"
#include "content/common/pepper_renderer_instance_data.h"
#include "content/common/view_message_enums.h"
#include "content/common/webplugin_geometry.h"
#include "content/port/common/input_event_ack_state.h"
#include "content/public/common/color_suggestion.h"
#include "content/public/common/common_param_traits.h"
#include "content/public/common/context_menu_params.h"
#include "content/public/common/favicon_url.h"
#include "content/public/common/file_chooser_params.h"
#include "content/public/common/frame_navigate_params.h"
#include "content/public/common/javascript_message_type.h"
#include "content/public/common/menu_item.h"
#include "content/public/common/page_state.h"
#include "content/public/common/page_zoom.h"
#include "content/public/common/referrer.h"
#include "content/public/common/renderer_preferences.h"
#include "content/public/common/stop_find_action.h"
#include "content/public/common/three_d_api_types.h"
#include "content/public/common/window_container_type.h"
#include "content/common/date_time_suggestion.h"
#include "ipc/ipc_channel_handle.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_platform_file.h"
#include "media/audio/audio_parameters.h"
#include "media/base/channel_layout.h"
#include "media/base/media_log_event.h"
#include "third_party/WebKit/public/platform/WebFloatPoint.h"
#include "third_party/WebKit/public/platform/WebFloatRect.h"
#include "third_party/WebKit/public/platform/WebScreenInfo.h"
#include "third_party/WebKit/public/web/WebCompositionUnderline.h"
#include "third_party/WebKit/public/web/WebFindOptions.h"
#include "third_party/WebKit/public/web/WebMediaPlayerAction.h"
#include "third_party/WebKit/public/web/WebPluginAction.h"
#include "third_party/WebKit/public/web/WebPopupType.h"
#include "third_party/WebKit/public/web/WebTextDirection.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/base/ime/text_input_mode.h"
#include "ui/base/ime/text_input_type.h"
#include "ui/base/ui_base_types.h"
#include "ui/gfx/point.h"
#include "ui/gfx/range/range.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/rect_f.h"
#include "ui/gfx/vector2d.h"
#include "ui/gfx/vector2d_f.h"
#include "ui/shell_dialogs/selected_file_info.h"

#if defined(OS_MACOSX)
#include "content/common/mac/font_descriptor.h"
#endif

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT

#define IPC_MESSAGE_START ViewMsgStart

IPC_ENUM_TRAITS(AccessibilityMode)
IPC_ENUM_TRAITS(ViewMsg_Navigate_Type::Value)
IPC_ENUM_TRAITS(blink::WebContextMenuData::MediaType)
IPC_ENUM_TRAITS(blink::WebMediaPlayerAction::Type)
IPC_ENUM_TRAITS(blink::WebPluginAction::Type)
IPC_ENUM_TRAITS(blink::WebPopupType)
IPC_ENUM_TRAITS(blink::WebTextDirection)
IPC_ENUM_TRAITS(WindowContainerType)
IPC_ENUM_TRAITS(content::FaviconURL::IconType)
IPC_ENUM_TRAITS(content::FileChooserParams::Mode)
IPC_ENUM_TRAITS(content::JavaScriptMessageType)
IPC_ENUM_TRAITS(content::MenuItem::Type)
IPC_ENUM_TRAITS(content::NavigationGesture)
IPC_ENUM_TRAITS(content::PageZoom)
IPC_ENUM_TRAITS(content::RendererPreferencesHintingEnum)
IPC_ENUM_TRAITS(content::RendererPreferencesSubpixelRenderingEnum)
IPC_ENUM_TRAITS_MAX_VALUE(content::TapMultipleTargetsStrategy,
                          content::TAP_MULTIPLE_TARGETS_STRATEGY_MAX)
IPC_ENUM_TRAITS(content::StopFindAction)
IPC_ENUM_TRAITS(content::ThreeDAPIType)
IPC_ENUM_TRAITS(media::ChannelLayout)
IPC_ENUM_TRAITS(media::MediaLogEvent::Type)
IPC_ENUM_TRAITS(ui::MenuSourceType)
IPC_ENUM_TRAITS_MAX_VALUE(ui::TextInputMode, ui::TEXT_INPUT_MODE_MAX)
IPC_ENUM_TRAITS(ui::TextInputType)

#if defined(OS_MACOSX)
IPC_STRUCT_TRAITS_BEGIN(FontDescriptor)
  IPC_STRUCT_TRAITS_MEMBER(font_name)
  IPC_STRUCT_TRAITS_MEMBER(font_point_size)
IPC_STRUCT_TRAITS_END()
#endif

IPC_STRUCT_TRAITS_BEGIN(blink::WebCompositionUnderline)
  IPC_STRUCT_TRAITS_MEMBER(startOffset)
  IPC_STRUCT_TRAITS_MEMBER(endOffset)
  IPC_STRUCT_TRAITS_MEMBER(color)
  IPC_STRUCT_TRAITS_MEMBER(thick)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(blink::WebFindOptions)
  IPC_STRUCT_TRAITS_MEMBER(forward)
  IPC_STRUCT_TRAITS_MEMBER(matchCase)
  IPC_STRUCT_TRAITS_MEMBER(findNext)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(blink::WebMediaPlayerAction)
  IPC_STRUCT_TRAITS_MEMBER(type)
  IPC_STRUCT_TRAITS_MEMBER(enable)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(blink::WebPluginAction)
  IPC_STRUCT_TRAITS_MEMBER(type)
  IPC_STRUCT_TRAITS_MEMBER(enable)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(blink::WebFloatPoint)
  IPC_STRUCT_TRAITS_MEMBER(x)
  IPC_STRUCT_TRAITS_MEMBER(y)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(blink::WebFloatRect)
  IPC_STRUCT_TRAITS_MEMBER(x)
  IPC_STRUCT_TRAITS_MEMBER(y)
  IPC_STRUCT_TRAITS_MEMBER(width)
  IPC_STRUCT_TRAITS_MEMBER(height)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(blink::WebScreenInfo)
  IPC_STRUCT_TRAITS_MEMBER(deviceScaleFactor)
  IPC_STRUCT_TRAITS_MEMBER(depth)
  IPC_STRUCT_TRAITS_MEMBER(depthPerComponent)
  IPC_STRUCT_TRAITS_MEMBER(isMonochrome)
  IPC_STRUCT_TRAITS_MEMBER(rect)
  IPC_STRUCT_TRAITS_MEMBER(availableRect)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(content::MenuItem)
  IPC_STRUCT_TRAITS_MEMBER(label)
  IPC_STRUCT_TRAITS_MEMBER(tool_tip)
  IPC_STRUCT_TRAITS_MEMBER(type)
  IPC_STRUCT_TRAITS_MEMBER(action)
  IPC_STRUCT_TRAITS_MEMBER(rtl)
  IPC_STRUCT_TRAITS_MEMBER(has_directional_override)
  IPC_STRUCT_TRAITS_MEMBER(enabled)
  IPC_STRUCT_TRAITS_MEMBER(checked)
  IPC_STRUCT_TRAITS_MEMBER(submenu)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(content::ColorSuggestion)
  IPC_STRUCT_TRAITS_MEMBER(color)
  IPC_STRUCT_TRAITS_MEMBER(label)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(content::DateTimeSuggestion)
  IPC_STRUCT_TRAITS_MEMBER(value)
  IPC_STRUCT_TRAITS_MEMBER(localized_value)
  IPC_STRUCT_TRAITS_MEMBER(label)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(content::ContextMenuParams)
  IPC_STRUCT_TRAITS_MEMBER(media_type)
  IPC_STRUCT_TRAITS_MEMBER(x)
  IPC_STRUCT_TRAITS_MEMBER(y)
  IPC_STRUCT_TRAITS_MEMBER(link_url)
  IPC_STRUCT_TRAITS_MEMBER(link_text)
  IPC_STRUCT_TRAITS_MEMBER(unfiltered_link_url)
  IPC_STRUCT_TRAITS_MEMBER(src_url)
  IPC_STRUCT_TRAITS_MEMBER(has_image_contents)
  IPC_STRUCT_TRAITS_MEMBER(page_url)
  IPC_STRUCT_TRAITS_MEMBER(keyword_url)
  IPC_STRUCT_TRAITS_MEMBER(frame_url)
  IPC_STRUCT_TRAITS_MEMBER(frame_page_state)
  IPC_STRUCT_TRAITS_MEMBER(media_flags)
  IPC_STRUCT_TRAITS_MEMBER(selection_text)
  IPC_STRUCT_TRAITS_MEMBER(misspelled_word)
  IPC_STRUCT_TRAITS_MEMBER(misspelling_hash)
  IPC_STRUCT_TRAITS_MEMBER(dictionary_suggestions)
  IPC_STRUCT_TRAITS_MEMBER(speech_input_enabled)
  IPC_STRUCT_TRAITS_MEMBER(spellcheck_enabled)
  IPC_STRUCT_TRAITS_MEMBER(is_editable)
  IPC_STRUCT_TRAITS_MEMBER(writing_direction_default)
  IPC_STRUCT_TRAITS_MEMBER(writing_direction_left_to_right)
  IPC_STRUCT_TRAITS_MEMBER(writing_direction_right_to_left)
  IPC_STRUCT_TRAITS_MEMBER(edit_flags)
  IPC_STRUCT_TRAITS_MEMBER(security_info)
  IPC_STRUCT_TRAITS_MEMBER(frame_charset)
  IPC_STRUCT_TRAITS_MEMBER(referrer_policy)
  IPC_STRUCT_TRAITS_MEMBER(custom_context)
  IPC_STRUCT_TRAITS_MEMBER(custom_items)
  IPC_STRUCT_TRAITS_MEMBER(source_type)
#if defined(OS_ANDROID)
  IPC_STRUCT_TRAITS_MEMBER(selection_start)
  IPC_STRUCT_TRAITS_MEMBER(selection_end)
#endif
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(content::CustomContextMenuContext)
  IPC_STRUCT_TRAITS_MEMBER(is_pepper_menu)
  IPC_STRUCT_TRAITS_MEMBER(request_id)
  IPC_STRUCT_TRAITS_MEMBER(render_widget_id)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(content::FaviconURL)
  IPC_STRUCT_TRAITS_MEMBER(icon_url)
  IPC_STRUCT_TRAITS_MEMBER(icon_type)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(content::FileChooserParams)
  IPC_STRUCT_TRAITS_MEMBER(mode)
  IPC_STRUCT_TRAITS_MEMBER(title)
  IPC_STRUCT_TRAITS_MEMBER(default_file_name)
  IPC_STRUCT_TRAITS_MEMBER(accept_types)
#if defined(OS_ANDROID)
  IPC_STRUCT_TRAITS_MEMBER(capture)
#endif
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(content::FrameNavigateParams)
  IPC_STRUCT_TRAITS_MEMBER(page_id)
  IPC_STRUCT_TRAITS_MEMBER(url)
  IPC_STRUCT_TRAITS_MEMBER(base_url)
  IPC_STRUCT_TRAITS_MEMBER(referrer)
  IPC_STRUCT_TRAITS_MEMBER(transition)
  IPC_STRUCT_TRAITS_MEMBER(redirects)
  IPC_STRUCT_TRAITS_MEMBER(should_update_history)
  IPC_STRUCT_TRAITS_MEMBER(searchable_form_url)
  IPC_STRUCT_TRAITS_MEMBER(searchable_form_encoding)
  IPC_STRUCT_TRAITS_MEMBER(contents_mime_type)
  IPC_STRUCT_TRAITS_MEMBER(socket_address)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(content::PepperRendererInstanceData)
  IPC_STRUCT_TRAITS_MEMBER(render_process_id)
  IPC_STRUCT_TRAITS_MEMBER(render_view_id)
  IPC_STRUCT_TRAITS_MEMBER(document_url)
  IPC_STRUCT_TRAITS_MEMBER(plugin_url)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(content::RendererPreferences)
  IPC_STRUCT_TRAITS_MEMBER(can_accept_load_drops)
  IPC_STRUCT_TRAITS_MEMBER(should_antialias_text)
  IPC_STRUCT_TRAITS_MEMBER(hinting)
  IPC_STRUCT_TRAITS_MEMBER(use_autohinter)
  IPC_STRUCT_TRAITS_MEMBER(use_bitmaps)
  IPC_STRUCT_TRAITS_MEMBER(subpixel_rendering)
  IPC_STRUCT_TRAITS_MEMBER(use_subpixel_positioning)
  IPC_STRUCT_TRAITS_MEMBER(focus_ring_color)
  IPC_STRUCT_TRAITS_MEMBER(thumb_active_color)
  IPC_STRUCT_TRAITS_MEMBER(thumb_inactive_color)
  IPC_STRUCT_TRAITS_MEMBER(track_color)
  IPC_STRUCT_TRAITS_MEMBER(active_selection_bg_color)
  IPC_STRUCT_TRAITS_MEMBER(active_selection_fg_color)
  IPC_STRUCT_TRAITS_MEMBER(inactive_selection_bg_color)
  IPC_STRUCT_TRAITS_MEMBER(inactive_selection_fg_color)
  IPC_STRUCT_TRAITS_MEMBER(browser_handles_non_local_top_level_requests)
  IPC_STRUCT_TRAITS_MEMBER(browser_handles_all_top_level_requests)
  IPC_STRUCT_TRAITS_MEMBER(browser_handles_all_top_level_link_clicks)
  IPC_STRUCT_TRAITS_MEMBER(caret_blink_interval)
  IPC_STRUCT_TRAITS_MEMBER(use_custom_colors)
  IPC_STRUCT_TRAITS_MEMBER(enable_referrers)
  IPC_STRUCT_TRAITS_MEMBER(enable_do_not_track)
  IPC_STRUCT_TRAITS_MEMBER(default_zoom_level)
  IPC_STRUCT_TRAITS_MEMBER(user_agent_override)
  IPC_STRUCT_TRAITS_MEMBER(accept_languages)
  IPC_STRUCT_TRAITS_MEMBER(report_frame_name_changes)
  IPC_STRUCT_TRAITS_MEMBER(touchpad_fling_profile)
  IPC_STRUCT_TRAITS_MEMBER(touchscreen_fling_profile)
  IPC_STRUCT_TRAITS_MEMBER(tap_multiple_targets_strategy)
  IPC_STRUCT_TRAITS_MEMBER(disable_client_blocked_error_page)
  IPC_STRUCT_TRAITS_MEMBER(plugin_fullscreen_allowed)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(content::CookieData)
  IPC_STRUCT_TRAITS_MEMBER(name)
  IPC_STRUCT_TRAITS_MEMBER(value)
  IPC_STRUCT_TRAITS_MEMBER(domain)
  IPC_STRUCT_TRAITS_MEMBER(path)
  IPC_STRUCT_TRAITS_MEMBER(expires)
  IPC_STRUCT_TRAITS_MEMBER(http_only)
  IPC_STRUCT_TRAITS_MEMBER(secure)
  IPC_STRUCT_TRAITS_MEMBER(session)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(content::WebPluginGeometry)
  IPC_STRUCT_TRAITS_MEMBER(window)
  IPC_STRUCT_TRAITS_MEMBER(window_rect)
  IPC_STRUCT_TRAITS_MEMBER(clip_rect)
  IPC_STRUCT_TRAITS_MEMBER(cutout_rects)
  IPC_STRUCT_TRAITS_MEMBER(rects_valid)
  IPC_STRUCT_TRAITS_MEMBER(visible)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(media::MediaLogEvent)
  IPC_STRUCT_TRAITS_MEMBER(id)
  IPC_STRUCT_TRAITS_MEMBER(type)
  IPC_STRUCT_TRAITS_MEMBER(params)
  IPC_STRUCT_TRAITS_MEMBER(time)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(ui::SelectedFileInfo)
  IPC_STRUCT_TRAITS_MEMBER(file_path)
  IPC_STRUCT_TRAITS_MEMBER(local_path)
  IPC_STRUCT_TRAITS_MEMBER(display_name)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(content::BrowserRenderingStats)
  IPC_STRUCT_TRAITS_MEMBER(input_event_count)
  IPC_STRUCT_TRAITS_MEMBER(total_input_latency)
  IPC_STRUCT_TRAITS_MEMBER(touch_ui_count)
  IPC_STRUCT_TRAITS_MEMBER(total_touch_ui_latency)
  IPC_STRUCT_TRAITS_MEMBER(touch_acked_count)
  IPC_STRUCT_TRAITS_MEMBER(total_touch_acked_latency)
  IPC_STRUCT_TRAITS_MEMBER(scroll_update_count)
  IPC_STRUCT_TRAITS_MEMBER(total_scroll_update_latency)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_BEGIN(ViewHostMsg_CreateWindow_Params)
  
  IPC_STRUCT_MEMBER(int, opener_id)

  
  IPC_STRUCT_MEMBER(bool, user_gesture)

  
  IPC_STRUCT_MEMBER(WindowContainerType, window_container_type)

  
  IPC_STRUCT_MEMBER(int64, session_storage_namespace_id)

  
  
  IPC_STRUCT_MEMBER(base::string16, frame_name)

  
  IPC_STRUCT_MEMBER(int64, opener_frame_id)

  
  IPC_STRUCT_MEMBER(GURL, opener_url)

  
  IPC_STRUCT_MEMBER(GURL, opener_top_level_frame_url)

  
  IPC_STRUCT_MEMBER(GURL, opener_security_origin)

  
  
  IPC_STRUCT_MEMBER(bool, opener_suppressed)

  
  IPC_STRUCT_MEMBER(WindowOpenDisposition, disposition)

  
  
  IPC_STRUCT_MEMBER(GURL, target_url)

  
  
  IPC_STRUCT_MEMBER(content::Referrer, referrer)

  
  IPC_STRUCT_MEMBER(blink::WebWindowFeatures, features)

  
  
  
  IPC_STRUCT_MEMBER(std::vector<base::string16>, additional_features)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(ViewHostMsg_CreateWorker_Params)
  
  IPC_STRUCT_MEMBER(GURL, url)

  
  IPC_STRUCT_MEMBER(base::string16, name)

  
  IPC_STRUCT_MEMBER(unsigned long long, document_id)

  
  IPC_STRUCT_MEMBER(int, render_view_route_id)

  
  
  IPC_STRUCT_MEMBER(int, route_id)

  
  
  IPC_STRUCT_MEMBER(int64, script_resource_appcache_id)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(ViewHostMsg_DateTimeDialogValue_Params)
  IPC_STRUCT_MEMBER(ui::TextInputType, dialog_type)
  IPC_STRUCT_MEMBER(double, dialog_value)
  IPC_STRUCT_MEMBER(double, minimum)
  IPC_STRUCT_MEMBER(double, maximum)
  IPC_STRUCT_MEMBER(double, step)
  IPC_STRUCT_MEMBER(std::vector<content::DateTimeSuggestion>, suggestions)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(ViewHostMsg_DidFailProvisionalLoadWithError_Params)
  
  IPC_STRUCT_MEMBER(int64, frame_id)
  
  IPC_STRUCT_MEMBER(base::string16, frame_unique_name)
  
  IPC_STRUCT_MEMBER(bool, is_main_frame)
  
  IPC_STRUCT_MEMBER(int, error_code)
  
  
  IPC_STRUCT_MEMBER(base::string16, error_description)
  
  IPC_STRUCT_MEMBER(GURL, url)
  
  
  IPC_STRUCT_MEMBER(bool, showing_repost_interstitial)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN_WITH_PARENT(ViewHostMsg_FrameNavigate_Params,
                             content::FrameNavigateParams)
  IPC_STRUCT_TRAITS_PARENT(content::FrameNavigateParams)
  
  
  IPC_STRUCT_MEMBER(int64, frame_id)

  
  IPC_STRUCT_MEMBER(base::string16, frame_unique_name)

  
  
  IPC_STRUCT_MEMBER(std::string, security_info)

  
  IPC_STRUCT_MEMBER(content::NavigationGesture, gesture)

  
  IPC_STRUCT_MEMBER(bool, is_post)

  
  IPC_STRUCT_MEMBER(int64, post_id)

  
  
  
  IPC_STRUCT_MEMBER(bool, was_within_same_page)

  
  IPC_STRUCT_MEMBER(int, http_status_code)

  
  
  IPC_STRUCT_MEMBER(bool, was_fetched_via_proxy)

  
  IPC_STRUCT_MEMBER(content::PageState, page_state)

  
  IPC_STRUCT_MEMBER(GURL, original_request_url)

  
  IPC_STRUCT_MEMBER(bool, is_overriding_user_agent)

  
  
  IPC_STRUCT_MEMBER(bool, history_list_was_cleared)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(ViewHostMsg_OpenURL_Params)
  IPC_STRUCT_MEMBER(GURL, url)
  IPC_STRUCT_MEMBER(content::Referrer, referrer)
  IPC_STRUCT_MEMBER(WindowOpenDisposition, disposition)
  IPC_STRUCT_MEMBER(int64, frame_id)
  IPC_STRUCT_MEMBER(bool, should_replace_current_entry)
  IPC_STRUCT_MEMBER(bool, user_gesture)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(ViewHostMsg_SelectionBounds_Params)
  IPC_STRUCT_MEMBER(gfx::Rect, anchor_rect)
  IPC_STRUCT_MEMBER(blink::WebTextDirection, anchor_dir)
  IPC_STRUCT_MEMBER(gfx::Rect, focus_rect)
  IPC_STRUCT_MEMBER(blink::WebTextDirection, focus_dir)
  IPC_STRUCT_MEMBER(bool, is_anchor_first)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(ViewHostMsg_ShowPopup_Params)
  
  IPC_STRUCT_MEMBER(gfx::Rect, bounds)

  
  IPC_STRUCT_MEMBER(int, item_height)

  
  IPC_STRUCT_MEMBER(double, item_font_size)

  
  IPC_STRUCT_MEMBER(int, selected_item)

  
  IPC_STRUCT_MEMBER(std::vector<content::MenuItem>, popup_items)

  
  IPC_STRUCT_MEMBER(bool, right_aligned)

  
  IPC_STRUCT_MEMBER(bool, allow_multiple_selection)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(ViewHostMsg_TextInputState_Params)
  
  IPC_STRUCT_MEMBER(ui::TextInputType, type)

  
  IPC_STRUCT_MEMBER(std::string, value)

  
  
  IPC_STRUCT_MEMBER(int, selection_start)

  
  
  IPC_STRUCT_MEMBER(int, selection_end)

  
  IPC_STRUCT_MEMBER(int, composition_start)

  
  IPC_STRUCT_MEMBER(int, composition_end)

  
  IPC_STRUCT_MEMBER(bool, can_compose_inline)

  
  
  
  IPC_STRUCT_MEMBER(bool, show_ime_if_needed)

  
  IPC_STRUCT_MEMBER(bool, require_ack)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(ViewHostMsg_UpdateRect_Params)
  
  
  IPC_STRUCT_MEMBER(TransportDIB::Id, bitmap)

  
  IPC_STRUCT_MEMBER(gfx::Rect, bitmap_rect)

  
  
  
  IPC_STRUCT_MEMBER(gfx::Vector2d, scroll_delta)

  
  IPC_STRUCT_MEMBER(gfx::Rect, scroll_rect)

  
  IPC_STRUCT_MEMBER(gfx::Vector2d, scroll_offset)

  
  
  IPC_STRUCT_MEMBER(std::vector<gfx::Rect>, copy_rects)

  
  
  
  
  
  IPC_STRUCT_MEMBER(gfx::Size, view_size)

  
  IPC_STRUCT_MEMBER(std::vector<content::WebPluginGeometry>,
                    plugin_window_moves)

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  IPC_STRUCT_MEMBER(int, flags)

  
  
  IPC_STRUCT_MEMBER(bool, needs_ack)

  
  
  IPC_STRUCT_MEMBER(float, scale_factor)

  
  
  IPC_STRUCT_MEMBER(ui::LatencyInfo, latency_info)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(ViewMsg_Navigate_Params)
  
  
  
  
  IPC_STRUCT_MEMBER(int32, page_id)

  
  
  
  IPC_STRUCT_MEMBER(int, pending_history_list_offset)

  
  
  IPC_STRUCT_MEMBER(int, current_history_list_offset)
  IPC_STRUCT_MEMBER(int, current_history_list_length)

  
  
  
  IPC_STRUCT_MEMBER(bool, should_clear_history_list)

  
  IPC_STRUCT_MEMBER(GURL, url)

  
  
  IPC_STRUCT_MEMBER(GURL, base_url_for_data_url)

  
  
  IPC_STRUCT_MEMBER(GURL, history_url_for_data_url)

  
  
  IPC_STRUCT_MEMBER(content::Referrer, referrer)

  
  
  IPC_STRUCT_MEMBER(std::vector<GURL>, redirects)

  
  IPC_STRUCT_MEMBER(content::PageTransition, transition)

  
  
  
  IPC_STRUCT_MEMBER(bool, should_replace_current_entry)

  
  IPC_STRUCT_MEMBER(content::PageState, page_state)

  
  IPC_STRUCT_MEMBER(ViewMsg_Navigate_Type::Value, navigation_type)

  
  IPC_STRUCT_MEMBER(base::Time, request_time)

  
  IPC_STRUCT_MEMBER(std::string, extra_headers)

  
  
  
  
  IPC_STRUCT_MEMBER(int, transferred_request_child_id)
  IPC_STRUCT_MEMBER(int, transferred_request_request_id)

  
  IPC_STRUCT_MEMBER(bool, allow_download)

  
  IPC_STRUCT_MEMBER(bool, is_overriding_user_agent)

  
  IPC_STRUCT_MEMBER(bool, is_post)

  
  
  IPC_STRUCT_MEMBER(std::vector<unsigned char>, browser_initiated_post_data)

  
  
  IPC_STRUCT_MEMBER(bool, can_load_local_resources)

  
  IPC_STRUCT_MEMBER(std::string, frame_to_navigate)

  
  IPC_STRUCT_MEMBER(base::TimeTicks, browser_navigation_start)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(ViewMsg_New_Params)
  
  IPC_STRUCT_MEMBER(content::RendererPreferences, renderer_preferences)

  
  IPC_STRUCT_MEMBER(WebPreferences, web_preferences)

  
  IPC_STRUCT_MEMBER(int32, view_id)

  
  IPC_STRUCT_MEMBER(int32, main_frame_routing_id)

  
  IPC_STRUCT_MEMBER(int32, surface_id)

  
  IPC_STRUCT_MEMBER(int64, session_storage_namespace_id)

  
  IPC_STRUCT_MEMBER(base::string16, frame_name)

  
  
  IPC_STRUCT_MEMBER(int, opener_route_id)

  
  IPC_STRUCT_MEMBER(bool, swapped_out)

  
  IPC_STRUCT_MEMBER(bool, hidden)

  
  
  
  IPC_STRUCT_MEMBER(int32, next_page_id)

  
  IPC_STRUCT_MEMBER(blink::WebScreenInfo, screen_info)

  
  IPC_STRUCT_MEMBER(AccessibilityMode, accessibility_mode)

  
  
  
  IPC_STRUCT_MEMBER(bool, allow_partial_swap)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(ViewMsg_PostMessage_Params)
  
  IPC_STRUCT_MEMBER(base::string16, data)

  
  
  
  IPC_STRUCT_MEMBER(int, source_routing_id)

  
  IPC_STRUCT_MEMBER(base::string16, source_origin)

  
  IPC_STRUCT_MEMBER(base::string16, target_origin)

  
  IPC_STRUCT_MEMBER(std::vector<int>, message_port_ids)
  IPC_STRUCT_MEMBER(std::vector<int>, new_routing_ids)
IPC_STRUCT_END()


IPC_MESSAGE_ROUTED0(ViewMsg_CancelDateTimeDialog)

IPC_MESSAGE_ROUTED1(ViewMsg_GetAllSavableResourceLinksForCurrentPage,
                    GURL )

IPC_MESSAGE_ROUTED3(ViewMsg_GetSerializedHtmlDataForCurrentPageWithLocalLinks,
                    std::vector<GURL> ,
                    std::vector<base::FilePath> ,
                    base::FilePath )

IPC_MESSAGE_ROUTED1(ViewMsg_LockMouse_ACK,
                    bool )
IPC_MESSAGE_ROUTED0(ViewMsg_MouseLockLost)

IPC_MESSAGE_ROUTED1(ViewMsg_OrientationChangeEvent,
                    int )

IPC_MESSAGE_ROUTED2(ViewMsg_UpdateVSyncParameters,
                    base::TimeTicks ,
                    base::TimeDelta )

IPC_MESSAGE_ROUTED1(ViewMsg_SetName,
                    std::string )

IPC_MESSAGE_ROUTED2(ViewMsg_SetHistoryLengthAndPrune,
                    int, 
                    int32 )

IPC_MESSAGE_CONTROL1(ViewMsg_SetRendererProcessID,
                     base::ProcessId )

IPC_MESSAGE_CONTROL1(ViewMsg_New,
                     ViewMsg_New_Params)

IPC_MESSAGE_ROUTED0(ViewMsg_CreatingNew_ACK)

IPC_MESSAGE_ROUTED1(ViewMsg_SetRendererPrefs,
                    content::RendererPreferences)

IPC_MESSAGE_ROUTED1(ViewMsg_UpdateWebPreferences,
                    WebPreferences)

IPC_MESSAGE_ROUTED0(ViewMsg_TimezoneChange)

IPC_MESSAGE_ROUTED0(ViewMsg_Close)

IPC_STRUCT_BEGIN(ViewMsg_Resize_Params)
  IPC_STRUCT_MEMBER(blink::WebScreenInfo, screen_info)
  IPC_STRUCT_MEMBER(gfx::Size, new_size)
  IPC_STRUCT_MEMBER(gfx::Size, physical_backing_size)
  IPC_STRUCT_MEMBER(float, overdraw_bottom_height)
  IPC_STRUCT_MEMBER(gfx::Rect, resizer_rect)
  IPC_STRUCT_MEMBER(bool, is_fullscreen)
IPC_STRUCT_END()

IPC_MESSAGE_ROUTED1(ViewMsg_Resize,
                    ViewMsg_Resize_Params )

IPC_MESSAGE_ROUTED1(ViewMsg_ChangeResizeRect,
                    gfx::Rect )

IPC_MESSAGE_ROUTED0(ViewMsg_WasHidden)

IPC_MESSAGE_ROUTED1(ViewMsg_WasShown,
                    bool )

IPC_MESSAGE_ROUTED0(ViewMsg_WasSwappedOut)

IPC_MESSAGE_ROUTED4(ViewMsg_PaintAtSize,
                    TransportDIB::Handle ,
                    int ,
                    gfx::Size ,
                    gfx::Size )

IPC_MESSAGE_ROUTED0(ViewMsg_UpdateRect_ACK)

IPC_MESSAGE_ROUTED0(ViewMsg_SwapBuffers_ACK)

IPC_MESSAGE_ROUTED1(ViewMsg_SetInitialFocus,
                    bool )

IPC_MESSAGE_ROUTED2(ViewMsg_CustomContextMenuAction,
                    content::CustomContextMenuContext ,
                    unsigned )

IPC_MESSAGE_ROUTED1(ViewMsg_ContextMenuClosed,
                    content::CustomContextMenuContext )

IPC_MESSAGE_ROUTED1(ViewMsg_ShowContextMenu,
                    gfx::Point )

IPC_MESSAGE_ROUTED1(ViewMsg_Navigate, ViewMsg_Navigate_Params)

IPC_MESSAGE_ROUTED0(ViewMsg_Stop)

IPC_MESSAGE_ROUTED0(ViewMsg_ReloadFrame)

IPC_MESSAGE_ROUTED3(ViewMsg_Find,
                    int ,
                    base::string16 ,
                    blink::WebFindOptions)

IPC_MESSAGE_ROUTED1(ViewMsg_StopFinding,
                    content::StopFindAction )

IPC_MESSAGE_ROUTED1(ViewMsg_SetBrowserRenderingStats,
                    content::BrowserRenderingStats )

IPC_MESSAGE_ROUTED1(ViewMsg_ReplaceDateTime,
                    double )

IPC_MESSAGE_ROUTED2(ViewMsg_CopyImageAt,
                    int ,
                    int )

IPC_MESSAGE_ROUTED2(ViewMsg_MediaPlayerActionAt,
                    gfx::Point, 
                    blink::WebMediaPlayerAction)

IPC_MESSAGE_ROUTED2(ViewMsg_PluginActionAt,
                    gfx::Point, 
                    blink::WebPluginAction)

IPC_MESSAGE_ROUTED4(ViewMsg_ScriptEvalRequest,
                    base::string16,  
                    base::string16,  
                    int,  
                    bool  )

IPC_MESSAGE_ROUTED1(ViewMsg_PostMessageEvent,
                    ViewMsg_PostMessage_Params)

IPC_MESSAGE_ROUTED0(ViewMsg_DisownOpener)

IPC_MESSAGE_ROUTED2(ViewMsg_CSSInsertRequest,
                    base::string16,  
                    std::string  )

IPC_MESSAGE_ROUTED1(ViewMsg_Zoom,
                    content::PageZoom )

IPC_MESSAGE_ROUTED1(ViewMsg_SetZoomLevel,
                    double )

IPC_MESSAGE_ROUTED3(ViewMsg_ZoomFactor,
                    content::PageZoom,
                    int ,
                    int )

IPC_MESSAGE_ROUTED2(ViewMsg_SetZoomLevelForLoadingURL,
                    GURL ,
                    double )

IPC_MESSAGE_CONTROL3(ViewMsg_SetZoomLevelForCurrentURL,
                     std::string ,
                     std::string ,
                     double )

IPC_MESSAGE_ROUTED1(ViewMsg_SetPageEncoding,
                    std::string )

IPC_MESSAGE_ROUTED0(ViewMsg_ResetPageEncodingToDefault)

IPC_MESSAGE_ROUTED1(ViewMsg_AllowBindings,
                    int )

IPC_MESSAGE_ROUTED2(ViewMsg_SetWebUIProperty,
                    std::string ,
                    std::string )

IPC_MESSAGE_ROUTED1(ViewMsg_SetInputMethodActive,
                    bool )

IPC_MESSAGE_ROUTED0(ViewMsg_CandidateWindowShown)
IPC_MESSAGE_ROUTED0(ViewMsg_CandidateWindowUpdated)
IPC_MESSAGE_ROUTED0(ViewMsg_CandidateWindowHidden)

IPC_MESSAGE_ROUTED4(
    ViewMsg_ImeSetComposition,
    base::string16, 
    std::vector<blink::WebCompositionUnderline>, 
    int, 
    int )

IPC_MESSAGE_ROUTED3(ViewMsg_ImeConfirmComposition,
                    base::string16 ,
                    gfx::Range ,
                    bool )

IPC_MESSAGE_ROUTED3(ViewMsg_SetCompositionFromExistingText,
    int ,
    int ,
    std::vector<blink::WebCompositionUnderline> )

IPC_MESSAGE_ROUTED2(ViewMsg_SetEditableSelectionOffsets,
                    int ,
                    int )

IPC_MESSAGE_ROUTED2(ViewMsg_ExtendSelectionAndDelete,
                    int ,
                    int )

IPC_MESSAGE_ROUTED0(ViewMsg_UpdateTargetURL_ACK)


IPC_MESSAGE_ROUTED1(ViewMsg_SetAltErrorPageURL,
                    GURL)

IPC_MESSAGE_ROUTED2(ViewMsg_DidChooseColorResponse, unsigned, SkColor)

IPC_MESSAGE_ROUTED1(ViewMsg_DidEndColorChooser, unsigned)

IPC_MESSAGE_ROUTED1(ViewMsg_RunFileChooserResponse,
                    std::vector<ui::SelectedFileInfo>)

IPC_MESSAGE_ROUTED2(ViewMsg_EnumerateDirectoryResponse,
                    int ,
                    std::vector<base::FilePath> )

IPC_MESSAGE_ROUTED0(ViewMsg_CantFocus)

IPC_MESSAGE_ROUTED0(ViewMsg_ShouldClose)

IPC_MESSAGE_ROUTED0(ViewMsg_SuppressDialogsUntilSwapOut)

IPC_MESSAGE_ROUTED0(ViewMsg_SwapOut)

IPC_MESSAGE_ROUTED0(ViewMsg_ClosePage)

IPC_MESSAGE_ROUTED0(ViewMsg_ThemeChanged)

IPC_MESSAGE_ROUTED1(ViewMsg_Repaint,
                    gfx::Size )

IPC_MESSAGE_ROUTED0(ViewMsg_MoveOrResizeStarted)

IPC_MESSAGE_ROUTED2(ViewMsg_UpdateScreenRects,
                    gfx::Rect ,
                    gfx::Rect )

IPC_MESSAGE_ROUTED0(ViewMsg_Move_ACK)

IPC_MESSAGE_ROUTED0(ViewMsg_EnablePreferredSizeChangedMode)

IPC_MESSAGE_ROUTED2(ViewMsg_EnableAutoResize,
                    gfx::Size ,
                    gfx::Size )

IPC_MESSAGE_ROUTED1(ViewMsg_DisableAutoResize,
                    gfx::Size )

IPC_MESSAGE_ROUTED1(ViewMsg_SetTextDirection,
                    blink::WebTextDirection )

IPC_MESSAGE_ROUTED0(ViewMsg_ClearFocusedNode)

IPC_MESSAGE_ROUTED1(ViewMsg_SetBackground,
                    SkBitmap )

IPC_MESSAGE_ROUTED1(ViewMsg_DisableScrollbarsForSmallWindows,
                    gfx::Size )

IPC_MESSAGE_ROUTED1(ViewMsg_SetActive,
                    bool )

IPC_MESSAGE_ROUTED0(ViewMsg_WorkerCreated)

IPC_MESSAGE_CONTROL1(ViewMsg_NetworkStateChanged,
                     bool )

IPC_MESSAGE_ROUTED2(ViewMsg_PpapiBrokerChannelCreated,
                    base::ProcessId ,
                    IPC::ChannelHandle )

IPC_MESSAGE_ROUTED1(ViewMsg_PpapiBrokerPermissionResult,
                    bool )

IPC_MESSAGE_CONTROL1(ViewMsg_PurgePluginListCache,
                     bool )

IPC_MESSAGE_ROUTED0(ViewMsg_EnableViewSourceMode)

IPC_MESSAGE_ROUTED2(ViewMsg_SavePageAsMHTML,
                    int ,
                    IPC::PlatformFileForTransit )

IPC_MESSAGE_CONTROL1(ViewMsg_TempCrashWithData,
                     GURL )

IPC_MESSAGE_ROUTED1(ViewMsg_SetAccessibilityMode,
                    AccessibilityMode)

IPC_MESSAGE_ROUTED1(ViewMsg_ReleaseDisambiguationPopupDIB,
                    TransportDIB::Handle )

IPC_MESSAGE_ROUTED3(ViewMsg_WindowSnapshotCompleted,
                    int ,
                    gfx::Size ,
                    std::vector<unsigned char> )

#if defined(OS_MACOSX)
IPC_MESSAGE_CONTROL4(ViewMsg_UpdateScrollbarTheme,
                     float ,
                     float ,
                     bool ,
                     bool )
#endif

#if defined(OS_ANDROID)
IPC_MESSAGE_CONTROL1(ViewMsg_SetWebKitSharedTimersSuspended,
                     bool )

IPC_MESSAGE_ROUTED1(ViewMsg_FindMatchRects,
                    int )

IPC_MESSAGE_ROUTED2(ViewMsg_SelectPopupMenuItems,
                    bool ,
                    std::vector<int> )

IPC_MESSAGE_ROUTED0(ViewMsg_UndoScrollFocusedEditableNodeIntoView)

IPC_MESSAGE_ROUTED3(ViewMsg_UpdateTopControlsState,
                    bool ,
                    bool ,
                    bool )

IPC_MESSAGE_ROUTED0(ViewMsg_ShowImeIfNeeded)

IPC_MESSAGE_ROUTED1(ViewMsg_BeginFrame,
                    cc::BeginFrameArgs )

IPC_MESSAGE_ROUTED0(ViewMsg_ImeEventAck)

IPC_MESSAGE_ROUTED0(ViewMsg_PauseVideo);

IPC_MESSAGE_ROUTED1(ViewMsg_ExtractSmartClipData,
                    gfx::Rect )

#elif defined(OS_MACOSX)
IPC_MESSAGE_ROUTED1(ViewMsg_SetWindowVisibility,
                    bool )

IPC_MESSAGE_ROUTED2(ViewMsg_WindowFrameChanged,
                    gfx::Rect ,
                    gfx::Rect )

IPC_MESSAGE_ROUTED1(ViewMsg_SetInLiveResize,
                    bool )

IPC_MESSAGE_ROUTED2(ViewMsg_PluginImeCompositionCompleted,
                    base::string16 ,
                    int )

IPC_MESSAGE_ROUTED1(ViewMsg_SelectPopupMenuItem,
                    int )
#endif

IPC_MESSAGE_ROUTED2(ViewMsg_SwapCompositorFrameAck,
                    uint32 ,
                    cc::CompositorFrameAck )

IPC_MESSAGE_ROUTED2(ViewMsg_ReclaimCompositorResources,
                    uint32 ,
                    cc::CompositorFrameAck )

IPC_MESSAGE_ROUTED1(ViewMsg_Snapshot,
                    gfx::Rect )


IPC_SYNC_MESSAGE_CONTROL1_4(ViewHostMsg_CreateWindow,
                            ViewHostMsg_CreateWindow_Params,
                            int ,
                            int ,
                            int32 ,
                            int64 )

IPC_SYNC_MESSAGE_CONTROL2_2(ViewHostMsg_CreateWidget,
                            int ,
                            blink::WebPopupType ,
                            int ,
                            int32 )

IPC_SYNC_MESSAGE_CONTROL1_2(ViewHostMsg_CreateFullscreenWidget,
                            int ,
                            int ,
                            int32 )

IPC_SYNC_MESSAGE_CONTROL0_1(ViewHostMsg_GenerateRoutingID,
                            int )

IPC_SYNC_MESSAGE_CONTROL0_2(ViewHostMsg_GetAudioHardwareConfig,
                            media::AudioParameters ,
                            media::AudioParameters )

IPC_SYNC_MESSAGE_CONTROL0_1(ViewHostMsg_GetCPUUsage,
                            int )

IPC_SYNC_MESSAGE_CONTROL0_2(ViewHostMsg_GetProcessMemorySizes,
                            size_t ,
                            size_t )

IPC_MESSAGE_ROUTED4(ViewHostMsg_ShowView,
                    int ,
                    WindowOpenDisposition ,
                    gfx::Rect ,
                    bool )

IPC_MESSAGE_ROUTED2(ViewHostMsg_ShowWidget,
                    int ,
                    gfx::Rect )

IPC_MESSAGE_ROUTED1(ViewHostMsg_ShowFullscreenWidget,
                    int )

IPC_SYNC_MESSAGE_ROUTED1_0(ViewHostMsg_RunModal,
                           int )

IPC_MESSAGE_ROUTED0(ViewHostMsg_RenderViewReady)

IPC_MESSAGE_ROUTED2(ViewHostMsg_RenderProcessGone,
                    int, 
                    int )

IPC_MESSAGE_ROUTED0(ViewHostMsg_Close)

IPC_MESSAGE_ROUTED0(ViewHostMsg_UpdateScreenRects_ACK)

IPC_MESSAGE_ROUTED1(ViewHostMsg_RequestMove,
                    gfx::Rect )

IPC_MESSAGE_ROUTED1(ViewHostMsg_FrameNavigate,
                    ViewHostMsg_FrameNavigate_Params)

IPC_MESSAGE_ROUTED1(ViewHostMsg_ContextMenu, content::ContextMenuParams)

IPC_MESSAGE_ROUTED1(ViewHostMsg_ShowPopup,
                    ViewHostMsg_ShowPopup_Params)

IPC_MESSAGE_ROUTED2(ViewHostMsg_ScriptEvalResponse,
                    int  ,
                    base::ListValue  )

IPC_MESSAGE_ROUTED5(ViewHostMsg_Find_Reply,
                    int ,
                    int ,
                    gfx::Rect ,
                    int ,
                    bool )

IPC_MESSAGE_ROUTED3(ViewHostMsg_ShouldClose_ACK,
                    bool ,
                    base::TimeTicks ,
                    base::TimeTicks )

IPC_MESSAGE_ROUTED0(ViewHostMsg_SwapOut_ACK)

IPC_MESSAGE_ROUTED0(ViewHostMsg_ClosePage_ACK)

IPC_MESSAGE_ROUTED4(ViewHostMsg_MediaNotification,
                    int64 ,
                    bool ,
                    bool ,
                    bool )

IPC_MESSAGE_ROUTED2(ViewHostMsg_UpdateState,
                    int32 ,
                    content::PageState )

IPC_MESSAGE_ROUTED1(ViewHostMsg_DocumentLoadedInFrame,
                    int64 )

IPC_MESSAGE_ROUTED3(ViewHostMsg_DidFinishLoad,
                    int64 ,
                    GURL ,
                    bool )

IPC_MESSAGE_ROUTED3(ViewHostMsg_UpdateTitle,
                    int32 ,
                    base::string16 ,
                    blink::WebTextDirection )

IPC_MESSAGE_ROUTED1(ViewHostMsg_UpdateEncoding,
                    std::string )

IPC_MESSAGE_ROUTED2(ViewHostMsg_UpdateTargetURL,
                    int32,
                    GURL)

IPC_MESSAGE_ROUTED0(ViewHostMsg_DidStartLoading)

IPC_MESSAGE_ROUTED0(ViewHostMsg_DidStopLoading)

IPC_MESSAGE_ROUTED1(ViewHostMsg_DidChangeLoadProgress,
                    double )

IPC_MESSAGE_ROUTED0(ViewHostMsg_DidDisownOpener)

IPC_MESSAGE_ROUTED0(ViewHostMsg_DocumentAvailableInMainFrame)

IPC_MESSAGE_ROUTED1(ViewHostMsg_DocumentOnLoadCompletedInMainFrame,
                    int32 )

IPC_MESSAGE_ROUTED5(ViewHostMsg_DidLoadResourceFromMemoryCache,
                    GURL ,
                    std::string  ,
                    std::string  ,
                    std::string  ,
                    ResourceType::Type )

IPC_MESSAGE_ROUTED0(ViewHostMsg_DidDisplayInsecureContent)

IPC_MESSAGE_ROUTED2(ViewHostMsg_DidRunInsecureContent,
                    std::string  ,
                    GURL         )

IPC_MESSAGE_ROUTED5(ViewHostMsg_DidFailLoadWithError,
                    int64 ,
                    GURL ,
                    bool ,
                    int ,
                    base::string16 )

IPC_MESSAGE_ROUTED1(ViewHostMsg_DidFailProvisionalLoadWithError,
                    ViewHostMsg_DidFailProvisionalLoadWithError_Params)

IPC_MESSAGE_ROUTED2(ViewHostMsg_PaintAtSize_ACK,
                    int ,
                    gfx::Size )

IPC_MESSAGE_ROUTED1(ViewHostMsg_UpdateRect,
                    ViewHostMsg_UpdateRect_Params)

IPC_MESSAGE_ROUTED0(ViewHostMsg_UpdateIsDelayed)

IPC_MESSAGE_ROUTED1(ViewHostMsg_DidActivateAcceleratedCompositing,
                    bool )

IPC_MESSAGE_ROUTED0(ViewHostMsg_Focus)
IPC_MESSAGE_ROUTED0(ViewHostMsg_Blur)

IPC_MESSAGE_ROUTED1(ViewHostMsg_FocusedNodeChanged,
    bool )

IPC_MESSAGE_ROUTED1(ViewHostMsg_SetCursor,
                    WebCursor)

IPC_MESSAGE_ROUTED3(ViewHostMsg_SetCookie,
                    GURL ,
                    GURL ,
                    std::string )

IPC_SYNC_MESSAGE_ROUTED2_1(ViewHostMsg_GetCookies,
                           GURL ,
                           GURL ,
                           std::string )

IPC_SYNC_MESSAGE_CONTROL2_1(ViewHostMsg_GetRawCookies,
                            GURL ,
                            GURL ,
                            std::vector<content::CookieData>
                                )

IPC_SYNC_MESSAGE_CONTROL2_0(ViewHostMsg_DeleteCookie,
                            GURL ,
                            std::string )

IPC_SYNC_MESSAGE_CONTROL2_1(ViewHostMsg_CookiesEnabled,
                            GURL ,
                            GURL ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL1_1(ViewHostMsg_GetPlugins,
    bool ,
    std::vector<content::WebPluginInfo> )

#if defined(OS_WIN)
IPC_MESSAGE_ROUTED1(ViewHostMsg_WindowlessPluginDummyWindowCreated,
                    gfx::NativeViewId )

IPC_MESSAGE_ROUTED1(ViewHostMsg_WindowlessPluginDummyWindowDestroyed,
                    gfx::NativeViewId )

IPC_SYNC_MESSAGE_CONTROL0_1(ViewHostMsg_GetMonitorColorProfile,
                            std::vector<char> )
#endif

IPC_SYNC_MESSAGE_CONTROL1_2(ViewHostMsg_ResolveProxy,
                            GURL ,
                            bool ,
                            std::string )

IPC_SYNC_MESSAGE_CONTROL1_1(ViewHostMsg_CreateWorker,
                            ViewHostMsg_CreateWorker_Params,
                            int )

IPC_SYNC_MESSAGE_CONTROL1_3(ViewHostMsg_LookupSharedWorker,
                            ViewHostMsg_CreateWorker_Params,
                            bool ,
                            int ,
                            bool )

IPC_MESSAGE_CONTROL1(ViewHostMsg_DocumentDetached,
                     uint64 )

IPC_MESSAGE_CONTROL1(ViewHostMsg_ForwardToWorker,
                     IPC::Message )

IPC_MESSAGE_ROUTED2(ViewHostMsg_AppCacheAccessed,
                    GURL ,
                    bool )

IPC_MESSAGE_ROUTED3(ViewHostMsg_DownloadUrl,
                    GURL     ,
                    content::Referrer ,
                    base::string16 )

IPC_MESSAGE_ROUTED1(ViewHostMsg_GoToEntryAtOffset,
                    int )

IPC_MESSAGE_ROUTED0(ViewHostMsg_RouteCloseEvent)

IPC_MESSAGE_ROUTED1(ViewHostMsg_RouteMessageEvent,
                    ViewMsg_PostMessage_Params)

IPC_SYNC_MESSAGE_ROUTED4_2(ViewHostMsg_RunJavaScriptMessage,
                           base::string16     ,
                           base::string16     ,
                           GURL         ,
                           content::JavaScriptMessageType ,
                           bool         ,
                           base::string16     )

IPC_MESSAGE_ROUTED1(ViewHostMsg_OpenURL, ViewHostMsg_OpenURL_Params)

IPC_MESSAGE_ROUTED1(ViewHostMsg_DidContentsPreferredSizeChange,
                    gfx::Size )

IPC_MESSAGE_ROUTED0(ViewHostMsg_DidChangeScrollOffset)

IPC_MESSAGE_ROUTED2(ViewHostMsg_DidChangeScrollOffsetPinningForMainFrame,
                    bool ,
                    bool )

IPC_MESSAGE_ROUTED2(ViewHostMsg_DidChangeScrollbarsForMainFrame,
                    bool ,
                    bool )

IPC_MESSAGE_ROUTED1(ViewHostMsg_DidChangeNumWheelEvents,
                    int )

IPC_MESSAGE_ROUTED1(ViewHostMsg_HasTouchEventHandlers,
                    bool )

IPC_MESSAGE_ROUTED3(ViewHostMsg_WebUISend,
                    GURL ,
                    std::string  ,
                    base::ListValue )

IPC_SYNC_MESSAGE_CONTROL1_3(ViewHostMsg_OpenChannelToPepperPlugin,
                            base::FilePath ,
                            IPC::ChannelHandle ,
                            base::ProcessId ,
                            int )

IPC_SYNC_MESSAGE_CONTROL4_0(
    ViewHostMsg_DidCreateOutOfProcessPepperInstance,
    int ,
    int32 ,
    content::PepperRendererInstanceData ,
    bool )

IPC_MESSAGE_CONTROL3(ViewHostMsg_DidDeleteOutOfProcessPepperInstance,
                     int ,
                     int32 ,
                     bool )

IPC_MESSAGE_CONTROL2(ViewHostMsg_DidCreateInProcessInstance,
                     int32 ,
                     content::PepperRendererInstanceData )

IPC_MESSAGE_CONTROL1(ViewHostMsg_DidDeleteInProcessInstance,
                     int32 )

IPC_MESSAGE_CONTROL2(ViewHostMsg_OpenChannelToPpapiBroker,
                     int ,
                     base::FilePath )

IPC_MESSAGE_ROUTED3(ViewHostMsg_RequestPpapiBrokerPermission,
                    int ,
                    GURL ,
                    base::FilePath )

#if defined(USE_X11)
IPC_SYNC_MESSAGE_ROUTED1_0(ViewHostMsg_CreatePluginContainer,
                           gfx::PluginWindowHandle )

IPC_SYNC_MESSAGE_ROUTED1_0(ViewHostMsg_DestroyPluginContainer,
                           gfx::PluginWindowHandle )
#endif

IPC_MESSAGE_ROUTED2(ViewHostMsg_SetTooltipText,
                    base::string16 ,
                    blink::WebTextDirection )

IPC_MESSAGE_ROUTED0(ViewHostMsg_SelectRange_ACK)
IPC_MESSAGE_ROUTED0(ViewHostMsg_MoveCaret_ACK)

IPC_MESSAGE_ROUTED3(ViewHostMsg_SelectionChanged,
                    base::string16 ,
                    size_t ,
                    gfx::Range )

IPC_MESSAGE_ROUTED1(ViewHostMsg_SelectionBoundsChanged,
                    ViewHostMsg_SelectionBounds_Params)

IPC_MESSAGE_ROUTED3(ViewHostMsg_OpenColorChooser,
                    int ,
                    SkColor ,
                    std::vector<content::ColorSuggestion> )

IPC_MESSAGE_ROUTED1(ViewHostMsg_EndColorChooser, int )

IPC_MESSAGE_ROUTED2(ViewHostMsg_SetSelectedColorInColorChooser,
                    int ,
                    SkColor )

IPC_MESSAGE_ROUTED1(ViewHostMsg_RunFileChooser,
                    content::FileChooserParams)

IPC_MESSAGE_ROUTED2(ViewHostMsg_EnumerateDirectory,
                    int ,
                    base::FilePath )

IPC_MESSAGE_ROUTED1(ViewHostMsg_TakeFocus,
                    bool )

IPC_MESSAGE_ROUTED1(ViewHostMsg_OpenDateTimeDialog,
                    ViewHostMsg_DateTimeDialogValue_Params )

IPC_MESSAGE_ROUTED3(ViewHostMsg_TextInputTypeChanged,
                    ui::TextInputType ,
                    ui::TextInputMode ,
                    bool )

IPC_MESSAGE_ROUTED1(ViewHostMsg_TextInputStateChanged,
                    ViewHostMsg_TextInputState_Params )

IPC_MESSAGE_ROUTED0(ViewHostMsg_ImeCancelComposition)

IPC_MESSAGE_ROUTED4(ViewHostMsg_AddMessageToConsole,
                    int32, 
                    base::string16, 
                    int32, 
                    base::string16  )

IPC_SYNC_MESSAGE_ROUTED3_2(ViewHostMsg_RunBeforeUnloadConfirm,
                           GURL,           
                           base::string16  ,
                           bool            ,
                           bool            ,
                           base::string16  )

IPC_MESSAGE_ROUTED3(ViewHostMsg_DidRedirectProvisionalLoad,
                    int ,
                    GURL ,
                    GURL )

IPC_MESSAGE_ROUTED3(ViewHostMsg_DidZoomURL,
                    double ,
                    bool ,
                    GURL )

IPC_MESSAGE_ROUTED3(ViewHostMsg_UpdateZoomLimits,
                    int ,
                    int ,
                    bool )

IPC_MESSAGE_CONTROL1(ViewHostMsg_SuddenTerminationChanged,
                     bool )

IPC_MESSAGE_ROUTED3(ViewHostMsg_UpdateFrameName,
                    int ,
                    bool ,
                    std::string )


IPC_STRUCT_BEGIN(ViewHostMsg_CompositorSurfaceBuffersSwapped_Params)
  IPC_STRUCT_MEMBER(int32, surface_id)
  IPC_STRUCT_MEMBER(uint64, surface_handle)
  IPC_STRUCT_MEMBER(int32, route_id)
  IPC_STRUCT_MEMBER(gfx::Size, size)
  IPC_STRUCT_MEMBER(float, scale_factor)
  IPC_STRUCT_MEMBER(int32, gpu_process_host_id)
  IPC_STRUCT_MEMBER(ui::LatencyInfo, latency_info)
IPC_STRUCT_END()

IPC_MESSAGE_ROUTED1(
    ViewHostMsg_CompositorSurfaceBuffersSwapped,
    ViewHostMsg_CompositorSurfaceBuffersSwapped_Params )

IPC_MESSAGE_ROUTED2(ViewHostMsg_SwapCompositorFrame,
                    uint32 ,
                    cc::CompositorFrame )

IPC_MESSAGE_ROUTED2(ViewHostMsg_DidOverscroll,
                    gfx::Vector2dF ,
                    gfx::Vector2dF )

IPC_MESSAGE_ROUTED2(ViewHostMsg_Snapshot,
                    bool, 
                    SkBitmap )


IPC_SYNC_MESSAGE_CONTROL3_1(ViewHostMsg_Keygen,
                            uint32 ,
                            std::string ,
                            GURL ,
                            std::string )

IPC_MESSAGE_CONTROL3(ViewHostMsg_DidGenerateCacheableMetadata,
                     GURL ,
                     double ,
                     std::vector<char> )

IPC_MESSAGE_ROUTED0(ViewHostMsg_JSOutOfMemory)

IPC_MESSAGE_ROUTED4(ViewHostMsg_RegisterProtocolHandler,
                    std::string ,
                    GURL ,
                    base::string16 ,
                    bool )

IPC_MESSAGE_ROUTED2(ViewHostMsg_UpdateInspectorSetting,
                    std::string,  
                    std::string )

IPC_MESSAGE_ROUTED1(ViewHostMsg_ToggleFullscreen,
                    bool )

IPC_MESSAGE_CONTROL1(ViewHostMsg_UserMetricsRecordAction,
                     std::string )

IPC_MESSAGE_CONTROL2(ViewHostMsg_SavedPageAsMHTML,
                     int ,
                     int64 )

IPC_MESSAGE_ROUTED3(ViewHostMsg_SendCurrentPageAllSavableResourceLinks,
                    std::vector<GURL> ,
                    std::vector<content::Referrer> ,
                    std::vector<GURL> )

IPC_MESSAGE_ROUTED3(ViewHostMsg_SendSerializedHtmlData,
                    GURL ,
                    std::string ,
                    int32 )

IPC_MESSAGE_CONTROL1(ViewHostMsg_MediaLogEvents,
                     std::vector<media::MediaLogEvent> )

IPC_MESSAGE_ROUTED3(ViewHostMsg_LockMouse,
                    bool ,
                    bool ,
                    bool )

IPC_MESSAGE_ROUTED0(ViewHostMsg_UnlockMouse)

IPC_MESSAGE_ROUTED0(ViewHostMsg_DidAccessInitialDocument)

IPC_MESSAGE_ROUTED2(ViewHostMsg_DomOperationResponse,
                    std::string  ,
                    int  )

IPC_MESSAGE_ROUTED3(ViewHostMsg_ShowDisambiguationPopup,
                    gfx::Rect, 
                    gfx::Size, 
                    TransportDIB::Id )

IPC_SYNC_MESSAGE_CONTROL3_1(ViewHostMsg_Are3DAPIsBlocked,
                            int ,
                            GURL ,
                            content::ThreeDAPIType ,
                            bool )

IPC_MESSAGE_CONTROL3(ViewHostMsg_DidLose3DContext,
                     GURL ,
                     content::ThreeDAPIType ,
                     int )

IPC_MESSAGE_ROUTED0(ViewHostMsg_WillInsertBody)

IPC_MESSAGE_ROUTED2(ViewHostMsg_UpdateFaviconURL,
                    int32 ,
                    std::vector<content::FaviconURL> )

IPC_MESSAGE_ROUTED1(ViewHostMsg_DidFirstVisuallyNonEmptyPaint,
                    int )

IPC_MESSAGE_CONTROL1(ViewHostMsg_Vibrate,
                     int64 )

IPC_MESSAGE_CONTROL0(ViewHostMsg_CancelVibration)

IPC_MESSAGE_ROUTED1(ViewHostMsg_FocusedNodeTouched,
                    bool )

IPC_MESSAGE_ROUTED3(ViewHostMsg_ShowValidationMessage,
                    gfx::Rect ,
                    base::string16 ,
                    base::string16 )

IPC_MESSAGE_ROUTED0(ViewHostMsg_HideValidationMessage)

IPC_MESSAGE_ROUTED1(ViewHostMsg_MoveValidationMessage,
                    gfx::Rect )

#if defined(OS_ANDROID)
IPC_MESSAGE_ROUTED3(ViewHostMsg_FindMatchRects_Reply,
                    int ,
                    std::vector<gfx::RectF> ,
                    gfx::RectF )

IPC_MESSAGE_ROUTED1(ViewHostMsg_StartContentIntent,
                    GURL )

IPC_MESSAGE_ROUTED1(ViewHostMsg_DidChangeBodyBackgroundColor,
                    uint32  )

IPC_MESSAGE_CONTROL3(ViewHostMsg_RunWebAudioMediaCodec,
                     base::SharedMemoryHandle ,
                     base::FileDescriptor ,
                     uint32_t )

IPC_MESSAGE_ROUTED1(ViewHostMsg_SetNeedsBeginFrame,
                    bool )

IPC_MESSAGE_ROUTED1(ViewHostMsg_SmartClipDataExtracted,
                    string16)

#elif defined(OS_MACOSX)
IPC_SYNC_MESSAGE_CONTROL1_3(ViewHostMsg_LoadFont,
                           FontDescriptor ,
                           uint32 ,
                           base::SharedMemoryHandle ,
                           uint32 )

IPC_MESSAGE_ROUTED2(ViewHostMsg_PluginFocusChanged,
                    bool, 
                    int )

IPC_MESSAGE_ROUTED0(ViewHostMsg_StartPluginIme)

#elif defined(OS_WIN)
IPC_SYNC_MESSAGE_CONTROL2_0(ViewHostMsg_PreCacheFontCharacters,
                            LOGFONT ,
                            base::string16 )
#endif

#if defined(OS_POSIX)
IPC_SYNC_MESSAGE_CONTROL2_1(ViewHostMsg_AllocTransportDIB,
                            uint32_t, 
                            bool, 
                            TransportDIB::Handle )

IPC_MESSAGE_CONTROL1(ViewHostMsg_FreeTransportDIB,
                     TransportDIB::Id )
#endif

#if defined(OS_MACOSX) || defined(OS_WIN) || defined(USE_AURA)
IPC_MESSAGE_ROUTED2(ViewHostMsg_ImeCompositionRangeChanged,
                    gfx::Range ,
                    std::vector<gfx::Rect> )
#endif

