// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/shared_memory.h"
#include "base/process/process.h"
#include "base/strings/string16.h"
#include "base/strings/stringprintf.h"
#include "base/values.h"
#include "build/build_config.h"
#include "chrome/common/autocomplete_match_type.h"
#include "chrome/common/common_param_traits.h"
#include "chrome/common/content_settings.h"
#include "chrome/common/content_settings_pattern.h"
#include "chrome/common/instant_types.h"
#include "chrome/common/ntp_logging_events.h"
#include "chrome/common/omnibox_focus_state.h"
#include "chrome/common/search_provider.h"
#include "chrome/common/translate/language_detection_details.h"
#include "chrome/common/translate/translate_errors.h"
#include "components/nacl/common/nacl_types.h"
#include "content/public/common/common_param_traits.h"
#include "content/public/common/referrer.h"
#include "content/public/common/top_controls_state.h"
#include "extensions/common/stack_frame.h"
#include "ipc/ipc_channel_handle.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_platform_file.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/WebKit/public/web/WebCache.h"
#include "third_party/WebKit/public/web/WebConsoleMessage.h"
#include "ui/base/window_open_disposition.h"
#include "ui/gfx/rect.h"

#ifndef CHROME_COMMON_RENDER_MESSAGES_H_
#define CHROME_COMMON_RENDER_MESSAGES_H_

class SkBitmap;

enum ViewHostMsg_JavaScriptStressTestControl_Commands {
  kJavaScriptStressTestSetStressRunType = 0,
  kJavaScriptStressTestPrepareStressRun = 1,
};

struct ChromeViewHostMsg_GetPluginInfo_Status {
  enum Value {
    kAllowed,
    kBlocked,
    kClickToPlay,
    kDisabled,
    kNotFound,
    kNPAPINotSupported,
    kOutdatedBlocked,
    kOutdatedDisallowed,
    kUnauthorized,
  };

  ChromeViewHostMsg_GetPluginInfo_Status() : value(kAllowed) {}

  Value value;
};

namespace IPC {

#if defined(OS_POSIX) && !defined(USE_AURA) && !defined(OS_ANDROID)

template <>
struct ParamTraits<gfx::NativeView> {
  typedef gfx::NativeView param_type;
  static void Write(Message* m, const param_type& p) {
    NOTIMPLEMENTED();
  }

  static bool Read(const Message* m, PickleIterator* iter, param_type* p) {
    NOTIMPLEMENTED();
    *p = NULL;
    return true;
  }

  static void Log(const param_type& p, std::string* l) {
    l->append(base::StringPrintf("<gfx::NativeView>"));
  }
};

#endif  

template <>
struct ParamTraits<ContentSettingsPattern> {
  typedef ContentSettingsPattern param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, PickleIterator* iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

}  

#endif  

#define IPC_MESSAGE_START ChromeMsgStart

IPC_ENUM_TRAITS(ChromeViewHostMsg_GetPluginInfo_Status::Value)
IPC_ENUM_TRAITS(OmniboxFocusChangeReason)
IPC_ENUM_TRAITS(OmniboxFocusState)
IPC_ENUM_TRAITS(search_provider::OSDDType)
IPC_ENUM_TRAITS(search_provider::InstallState)
IPC_ENUM_TRAITS(ThemeBackgroundImageAlignment)
IPC_ENUM_TRAITS(ThemeBackgroundImageTiling)
IPC_ENUM_TRAITS(TranslateErrors::Type)
IPC_ENUM_TRAITS(blink::WebConsoleMessage::Level)
IPC_ENUM_TRAITS(content::TopControlsState)

IPC_STRUCT_TRAITS_BEGIN(ChromeViewHostMsg_GetPluginInfo_Status)
IPC_STRUCT_TRAITS_MEMBER(value)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_BEGIN(ChromeViewHostMsg_GetPluginInfo_Output)
  IPC_STRUCT_MEMBER(ChromeViewHostMsg_GetPluginInfo_Status, status)
  IPC_STRUCT_MEMBER(content::WebPluginInfo, plugin)
  IPC_STRUCT_MEMBER(std::string, actual_mime_type)
  IPC_STRUCT_MEMBER(std::string, group_identifier)
  IPC_STRUCT_MEMBER(base::string16, group_name)
IPC_STRUCT_END()

IPC_STRUCT_TRAITS_BEGIN(ContentSettingsPattern::PatternParts)
  IPC_STRUCT_TRAITS_MEMBER(scheme)
  IPC_STRUCT_TRAITS_MEMBER(is_scheme_wildcard)
  IPC_STRUCT_TRAITS_MEMBER(host)
  IPC_STRUCT_TRAITS_MEMBER(has_domain_wildcard)
  IPC_STRUCT_TRAITS_MEMBER(port)
  IPC_STRUCT_TRAITS_MEMBER(is_port_wildcard)
  IPC_STRUCT_TRAITS_MEMBER(path)
  IPC_STRUCT_TRAITS_MEMBER(is_path_wildcard)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(ContentSettingPatternSource)
  IPC_STRUCT_TRAITS_MEMBER(primary_pattern)
  IPC_STRUCT_TRAITS_MEMBER(secondary_pattern)
  IPC_STRUCT_TRAITS_MEMBER(setting)
  IPC_STRUCT_TRAITS_MEMBER(source)
  IPC_STRUCT_TRAITS_MEMBER(incognito)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(InstantSuggestion)
  IPC_STRUCT_TRAITS_MEMBER(text)
  IPC_STRUCT_TRAITS_MEMBER(metadata)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(InstantMostVisitedItem)
  IPC_STRUCT_TRAITS_MEMBER(url)
  IPC_STRUCT_TRAITS_MEMBER(title)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(RendererContentSettingRules)
  IPC_STRUCT_TRAITS_MEMBER(image_rules)
  IPC_STRUCT_TRAITS_MEMBER(script_rules)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(RGBAColor)
  IPC_STRUCT_TRAITS_MEMBER(r)
  IPC_STRUCT_TRAITS_MEMBER(g)
  IPC_STRUCT_TRAITS_MEMBER(b)
  IPC_STRUCT_TRAITS_MEMBER(a)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(ThemeBackgroundInfo)
  IPC_STRUCT_TRAITS_MEMBER(using_default_theme)
  IPC_STRUCT_TRAITS_MEMBER(background_color)
  IPC_STRUCT_TRAITS_MEMBER(text_color)
  IPC_STRUCT_TRAITS_MEMBER(link_color)
  IPC_STRUCT_TRAITS_MEMBER(text_color_light)
  IPC_STRUCT_TRAITS_MEMBER(header_color)
  IPC_STRUCT_TRAITS_MEMBER(section_border_color)
  IPC_STRUCT_TRAITS_MEMBER(theme_id)
  IPC_STRUCT_TRAITS_MEMBER(image_horizontal_alignment)
  IPC_STRUCT_TRAITS_MEMBER(image_vertical_alignment)
  IPC_STRUCT_TRAITS_MEMBER(image_tiling)
  IPC_STRUCT_TRAITS_MEMBER(image_height)
  IPC_STRUCT_TRAITS_MEMBER(has_attribution)
  IPC_STRUCT_TRAITS_MEMBER(logo_alternate)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(blink::WebCache::ResourceTypeStat)
  IPC_STRUCT_TRAITS_MEMBER(count)
  IPC_STRUCT_TRAITS_MEMBER(size)
  IPC_STRUCT_TRAITS_MEMBER(liveSize)
  IPC_STRUCT_TRAITS_MEMBER(decodedSize)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(blink::WebCache::ResourceTypeStats)
  IPC_STRUCT_TRAITS_MEMBER(images)
  IPC_STRUCT_TRAITS_MEMBER(cssStyleSheets)
  IPC_STRUCT_TRAITS_MEMBER(scripts)
  IPC_STRUCT_TRAITS_MEMBER(xslStyleSheets)
  IPC_STRUCT_TRAITS_MEMBER(fonts)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(blink::WebCache::UsageStats)
  IPC_STRUCT_TRAITS_MEMBER(minDeadCapacity)
  IPC_STRUCT_TRAITS_MEMBER(maxDeadCapacity)
  IPC_STRUCT_TRAITS_MEMBER(capacity)
  IPC_STRUCT_TRAITS_MEMBER(liveSize)
  IPC_STRUCT_TRAITS_MEMBER(deadSize)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(LanguageDetectionDetails)
  IPC_STRUCT_TRAITS_MEMBER(time)
  IPC_STRUCT_TRAITS_MEMBER(url)
  IPC_STRUCT_TRAITS_MEMBER(content_language)
  IPC_STRUCT_TRAITS_MEMBER(cld_language)
  IPC_STRUCT_TRAITS_MEMBER(is_cld_reliable)
  IPC_STRUCT_TRAITS_MEMBER(html_root_language)
  IPC_STRUCT_TRAITS_MEMBER(adopted_language)
  IPC_STRUCT_TRAITS_MEMBER(contents)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(extensions::StackFrame)
  IPC_STRUCT_TRAITS_MEMBER(line_number)
  IPC_STRUCT_TRAITS_MEMBER(column_number)
  IPC_STRUCT_TRAITS_MEMBER(source)
  IPC_STRUCT_TRAITS_MEMBER(function)
IPC_STRUCT_TRAITS_END()

IPC_ENUM_TRAITS_MAX_VALUE(NTPLoggingEventType,
                          NTP_NUM_EVENT_TYPES)


IPC_MESSAGE_CONTROL3(ChromeViewMsg_SetCacheCapacities,
                     size_t ,
                     size_t ,
                     size_t )

IPC_MESSAGE_CONTROL1(ChromeViewMsg_ClearCache,
                     bool )

IPC_MESSAGE_CONTROL0(ChromeViewMsg_PurgeMemory)

IPC_MESSAGE_ROUTED4(ChromeViewMsg_WebUIJavaScript,
                    base::string16,  
                    base::string16,  
                    int,  
                    bool  )

IPC_MESSAGE_CONTROL1(ChromeViewMsg_SetContentSettingRules,
                     RendererContentSettingRules )

IPC_MESSAGE_ROUTED1(ChromeViewMsg_LoadBlockedPlugins,
                    std::string )

IPC_MESSAGE_CONTROL0(ChromeViewMsg_GetCacheResourceStats)

IPC_MESSAGE_CONTROL2(ChromeViewMsg_SetFieldTrialGroup,
                     std::string ,
                     std::string )

IPC_MESSAGE_CONTROL0(ChromeViewMsg_GetV8HeapStats)

IPC_MESSAGE_ROUTED3(ChromeViewMsg_HandleMessageFromExternalHost,
                    std::string ,
                    std::string ,
                    std::string )

IPC_MESSAGE_ROUTED0(ChromeViewMsg_DetermineIfPageSupportsInstant)

IPC_MESSAGE_ROUTED1(ChromeViewMsg_SearchBoxSetDisplayInstantResults,
                    bool )

IPC_MESSAGE_ROUTED2(ChromeViewMsg_SearchBoxFocusChanged,
                    OmniboxFocusState ,
                    OmniboxFocusChangeReason )

IPC_MESSAGE_ROUTED2(ChromeViewMsg_SearchBoxMarginChange,
                    int ,
                    int )

IPC_MESSAGE_ROUTED1(ChromeViewMsg_SearchBoxMostVisitedItemsChanged,
                    std::vector<InstantMostVisitedItem> )

IPC_MESSAGE_ROUTED1(ChromeViewMsg_SearchBoxPromoInformation,
                    bool )

IPC_MESSAGE_ROUTED1(ChromeViewMsg_SearchBoxSetInputInProgress,
                    bool )

IPC_MESSAGE_ROUTED1(ChromeViewMsg_SearchBoxSetSuggestionToPrefetch,
                    InstantSuggestion )

IPC_MESSAGE_ROUTED1(ChromeViewMsg_SearchBoxSubmit,
                    base::string16 )

IPC_MESSAGE_ROUTED1(ChromeViewMsg_SearchBoxThemeChanged,
                    ThemeBackgroundInfo )

IPC_MESSAGE_ROUTED2(ChromeViewMsg_ChromeIdentityCheckResult,
                    base::string16 ,
                    bool )

IPC_MESSAGE_ROUTED0(ChromeViewMsg_SearchBoxToggleVoiceSearch)

IPC_MESSAGE_ROUTED1(ChromeViewMsg_SetVisuallyDeemphasized,
                    bool )

IPC_MESSAGE_ROUTED4(ChromeViewMsg_TranslatePage,
                    int ,
                    std::string, 
                    std::string, 
                    std::string 
)

IPC_MESSAGE_ROUTED1(ChromeViewMsg_RevertTranslation,
                    int )

IPC_MESSAGE_CONTROL1(ChromeViewMsg_SetIsIncognitoProcess,
                     bool )

IPC_MESSAGE_ROUTED1(ChromeViewMsg_SetAllowDisplayingInsecureContent,
                    bool )

IPC_MESSAGE_ROUTED1(ChromeViewMsg_SetAllowRunningInsecureContent,
                    bool )

IPC_MESSAGE_ROUTED1(ChromeViewMsg_SetClientSidePhishingDetection,
                    bool )

IPC_MESSAGE_ROUTED2(ChromeViewMsg_RequestThumbnailForContextNode,
                    int ,
                    gfx::Size )

IPC_MESSAGE_ROUTED3(ChromeViewMsg_UpdateTopControlsState,
                    content::TopControlsState ,
                    content::TopControlsState ,
                    bool )


IPC_MESSAGE_ROUTED1(ChromeViewMsg_SetWindowFeatures,
                    blink::WebWindowFeatures )

IPC_MESSAGE_ROUTED2(ChromeViewHostMsg_RequestThumbnailForContextNode_ACK,
                    SkBitmap ,
                    gfx::Size )

#if defined(OS_ANDROID)
IPC_MESSAGE_ROUTED1(ChromeViewMsg_RetrieveWebappInformation,
                    GURL )
#endif  


IPC_SYNC_MESSAGE_CONTROL1_1(ChromeViewHostMsg_GetManagedAccounts,
                            GURL ,
                            std::vector<std::string> )

IPC_MESSAGE_CONTROL0(ChromeViewHostMsg_ShowBrowserAccountManagementUI)


IPC_MESSAGE_ROUTED2(ChromeViewMsg_JavaScriptStressTestControl,
                    int ,
                    int )

IPC_MESSAGE_ROUTED0(ChromeViewMsg_GetFPS)

IPC_MESSAGE_ROUTED0(ChromeViewMsg_SetAsInterstitial)

IPC_MESSAGE_ROUTED1(ChromeViewMsg_NetErrorInfo,
                    int )


IPC_MESSAGE_ROUTED2(ChromeViewHostMsg_TranslateLanguageDetermined,
                    LanguageDetectionDetails ,
                    bool )

IPC_MESSAGE_CONTROL1(ChromeViewHostMsg_UpdatedCacheStats,
                     blink::WebCache::UsageStats )

IPC_MESSAGE_ROUTED1(ChromeViewHostMsg_ContentBlocked,
                    ContentSettingsType )

IPC_SYNC_MESSAGE_CONTROL5_1(ChromeViewHostMsg_AllowDatabase,
                            int ,
                            GURL ,
                            GURL ,
                            base::string16 ,
                            base::string16 ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL4_1(ChromeViewHostMsg_AllowDOMStorage,
                            int ,
                            GURL ,
                            GURL ,
                            bool ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL3_1(ChromeViewHostMsg_AllowFileSystem,
                            int ,
                            GURL ,
                            GURL ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL4_1(ChromeViewHostMsg_AllowIndexedDB,
                            int ,
                            GURL ,
                            GURL ,
                            base::string16 ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL4_1(ChromeViewHostMsg_GetPluginInfo,
                            int ,
                            GURL ,
                            GURL ,
                            std::string ,
                            ChromeViewHostMsg_GetPluginInfo_Output )

IPC_SYNC_MESSAGE_CONTROL1_3(
    ChromeViewHostMsg_IsInternalPluginRegisteredForMimeType,
    std::string ,
    bool ,
    std::vector<base::string16> ,
    std::vector<base::string16> )

#if defined(ENABLE_PLUGIN_INSTALLATION)
IPC_MESSAGE_ROUTED2(ChromeViewHostMsg_FindMissingPlugin,
                    int ,
                    std::string )

IPC_MESSAGE_ROUTED1(ChromeViewHostMsg_RemovePluginPlaceholderHost,
                    int )

IPC_MESSAGE_ROUTED1(ChromeViewMsg_FoundMissingPlugin,
                    base::string16 )

IPC_MESSAGE_ROUTED0(ChromeViewMsg_DidNotFindMissingPlugin)

IPC_MESSAGE_ROUTED0(ChromeViewMsg_StartedDownloadingPlugin)

IPC_MESSAGE_ROUTED0(ChromeViewMsg_FinishedDownloadingPlugin)

IPC_MESSAGE_ROUTED1(ChromeViewMsg_ErrorDownloadingPlugin,
                    std::string )
#endif  

IPC_MESSAGE_ROUTED0(ChromeViewMsg_CancelledDownloadingPlugin)

IPC_MESSAGE_ROUTED0(ChromeViewHostMsg_OpenAboutPlugins)

IPC_MESSAGE_ROUTED1(ChromeViewHostMsg_CouldNotLoadPlugin,
                    base::FilePath )

IPC_MESSAGE_ROUTED1(ChromeViewHostMsg_NPAPINotSupported,
                    std::string )

IPC_MESSAGE_ROUTED0(ChromeViewMsg_NPAPINotSupported)

IPC_MESSAGE_ROUTED3(ChromeViewHostMsg_ForwardMessageToExternalHost,
                    std::string  ,
                    std::string  ,
                    std::string  )

IPC_MESSAGE_ROUTED3(ChromeViewHostMsg_PageHasOSDD,
                    int32 ,
                    GURL ,
                    search_provider::OSDDType)

IPC_SYNC_MESSAGE_ROUTED2_1(ChromeViewHostMsg_GetSearchProviderInstallState,
                           GURL ,
                           GURL ,
                           search_provider::InstallState )

IPC_MESSAGE_CONTROL2(ChromeViewHostMsg_V8HeapStats,
                     int ,
                     int )

IPC_MESSAGE_CONTROL1(ChromeViewHostMsg_DnsPrefetch,
                     std::vector<std::string> )

IPC_MESSAGE_CONTROL1(ChromeViewHostMsg_Preconnect,
                     GURL )

IPC_MESSAGE_ROUTED2(ChromeViewHostMsg_BlockedOutdatedPlugin,
                    int ,
                    std::string )

IPC_MESSAGE_ROUTED2(ChromeViewHostMsg_BlockedUnauthorizedPlugin,
                    base::string16 ,
                    std::string )

IPC_MESSAGE_CONTROL1(ChromeViewHostMsg_ResourceTypeStats,
                     blink::WebCache::ResourceTypeStats)

IPC_MESSAGE_ROUTED4(ChromeViewHostMsg_PageTranslated,
                    int,                  
                    std::string           ,
                    std::string           ,
                    TranslateErrors::Type )

IPC_MESSAGE_ROUTED0(ChromeViewHostMsg_CancelPrerenderForPrinting)

IPC_SYNC_MESSAGE_ROUTED1_1(ChromeViewHostMsg_CanTriggerClipboardRead,
                           GURL ,
                           bool )
IPC_SYNC_MESSAGE_ROUTED1_1(ChromeViewHostMsg_CanTriggerClipboardWrite,
                           GURL ,
                           bool )

IPC_SYNC_MESSAGE_ROUTED1_1(ChromeViewHostMsg_IsWebGLDebugRendererInfoAllowed,
                           GURL ,
                           bool )

IPC_MESSAGE_ROUTED0(ChromeViewHostMsg_DidBlockDisplayingInsecureContent)

IPC_MESSAGE_ROUTED0(ChromeViewHostMsg_DidBlockRunningInsecureContent)

#if defined(OS_ANDROID)
IPC_MESSAGE_ROUTED4(ChromeViewHostMsg_DidRetrieveWebappInformation,
                    bool ,
                    bool ,
                    bool ,
                    GURL )
#endif  

IPC_MESSAGE_ROUTED0(ChromeViewHostMsg_PDFHasUnsupportedFeature)

IPC_MESSAGE_ROUTED2(ChromeViewHostMsg_PDFSaveURLAs,
                    GURL ,
                    content::Referrer )

IPC_MESSAGE_ROUTED1(ChromeViewHostMsg_PDFUpdateContentRestrictions,
                    int )

IPC_SYNC_MESSAGE_ROUTED1_1(ChromeViewHostMsg_PDFModalPromptForPassword,
                           std::string ,
                           std::string )

IPC_MESSAGE_ROUTED1(ChromeViewHostMsg_FrameLoadingError,
                    int )

IPC_MESSAGE_ROUTED0(ChromeViewHostMsg_FrameLoadingCompleted)

IPC_MESSAGE_CONTROL2(ChromeViewHostMsg_FPS,
                     int ,
                     float )

IPC_MESSAGE_ROUTED2(ChromeViewHostMsg_LogEvent,
                    int ,
                    NTPLoggingEventType )

IPC_MESSAGE_ROUTED3(ChromeViewHostMsg_LogImpression,
                    int ,
                    int ,
                    base::string16 )

IPC_MESSAGE_ROUTED2(ChromeViewHostMsg_ChromeIdentityCheck,
                    int ,
                    base::string16 )

IPC_MESSAGE_ROUTED2(ChromeViewHostMsg_FocusOmnibox,
                    int ,
                    OmniboxFocusState )

IPC_MESSAGE_ROUTED2(ChromeViewHostMsg_PasteAndOpenDropdown,
                    int ,
                    base::string16 )

IPC_MESSAGE_ROUTED2(ChromeViewHostMsg_InstantSupportDetermined,
                    int ,
                    bool )

IPC_MESSAGE_ROUTED2(ChromeViewHostMsg_SearchBoxDeleteMostVisitedItem,
                    int ,
                    GURL )

IPC_MESSAGE_ROUTED4(ChromeViewHostMsg_SearchBoxNavigate,
                    int ,
                    GURL ,
                    WindowOpenDisposition ,
                    bool )

IPC_MESSAGE_ROUTED1(ChromeViewHostMsg_SearchBoxUndoAllMostVisitedDeletions,
                    int )

IPC_MESSAGE_ROUTED2(ChromeViewHostMsg_SearchBoxUndoMostVisitedDeletion,
                    int ,
                    GURL )

IPC_MESSAGE_ROUTED2(ChromeViewHostMsg_SetVoiceSearchSupported,
                    int ,
                    bool )

IPC_MESSAGE_CONTROL2(ChromeViewMsg_SetSearchURLs,
                     std::vector<GURL> ,
                     GURL )

IPC_MESSAGE_ROUTED4(ChromeViewHostMsg_DetailedConsoleMessageAdded,
                    base::string16 ,
                    base::string16 ,
                    extensions::StackTrace ,
                    int32 )
