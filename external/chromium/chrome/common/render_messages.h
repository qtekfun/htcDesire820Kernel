// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/process.h"
#include "base/shared_memory.h"
#include "base/string16.h"
#include "base/values.h"
#include "build/build_config.h"
#include "chrome/common/common_param_traits.h"
#include "chrome/common/instant_types.h"
#include "chrome/common/nacl_types.h"
#include "chrome/common/search_provider.h"
#include "chrome/common/thumbnail_score.h"
#include "chrome/common/translate_errors.h"
#include "chrome/common/view_types.h"
#include "content/common/common_param_traits.h"
#include "ipc/ipc_message_macros.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebCache.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebConsoleMessage.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/rect.h"

#ifndef CHROME_COMMON_RENDER_MESSAGES_H_
#define CHROME_COMMON_RENDER_MESSAGES_H_

enum ViewHostMsg_JavaScriptStressTestControl_Commands {
  kJavaScriptStressTestSetStressRunType = 0,
  kJavaScriptStressTestPrepareStressRun = 1,
};

namespace IPC {

#if defined(OS_POSIX)

template <>
struct ParamTraits<gfx::NativeView> {
  typedef gfx::NativeView param_type;
  static void Write(Message* m, const param_type& p) {
    NOTIMPLEMENTED();
  }

  static bool Read(const Message* m, void** iter, param_type* p) {
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
struct ParamTraits<ContentSettings> {
  typedef ContentSettings param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, void** iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

}  

#endif  

#define IPC_MESSAGE_START ChromeMsgStart

IPC_ENUM_TRAITS(InstantCompleteBehavior)
IPC_ENUM_TRAITS(search_provider::OSDDType)
IPC_ENUM_TRAITS(search_provider::InstallState)
IPC_ENUM_TRAITS(TranslateErrors::Type)
IPC_ENUM_TRAITS(ViewType::Type)
IPC_ENUM_TRAITS(WebKit::WebConsoleMessage::Level)

IPC_STRUCT_TRAITS_BEGIN(ThumbnailScore)
  IPC_STRUCT_TRAITS_MEMBER(boring_score)
  IPC_STRUCT_TRAITS_MEMBER(good_clipping)
  IPC_STRUCT_TRAITS_MEMBER(at_top)
  IPC_STRUCT_TRAITS_MEMBER(time_at_snapshot)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(WebKit::WebCache::ResourceTypeStat)
  IPC_STRUCT_TRAITS_MEMBER(count)
  IPC_STRUCT_TRAITS_MEMBER(size)
  IPC_STRUCT_TRAITS_MEMBER(liveSize)
  IPC_STRUCT_TRAITS_MEMBER(decodedSize)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(WebKit::WebCache::ResourceTypeStats)
  IPC_STRUCT_TRAITS_MEMBER(images)
  IPC_STRUCT_TRAITS_MEMBER(cssStyleSheets)
  IPC_STRUCT_TRAITS_MEMBER(scripts)
  IPC_STRUCT_TRAITS_MEMBER(xslStyleSheets)
  IPC_STRUCT_TRAITS_MEMBER(fonts)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(WebKit::WebCache::UsageStats)
  IPC_STRUCT_TRAITS_MEMBER(minDeadCapacity)
  IPC_STRUCT_TRAITS_MEMBER(maxDeadCapacity)
  IPC_STRUCT_TRAITS_MEMBER(capacity)
  IPC_STRUCT_TRAITS_MEMBER(liveSize)
  IPC_STRUCT_TRAITS_MEMBER(deadSize)
IPC_STRUCT_TRAITS_END()


IPC_MESSAGE_CONTROL3(ViewMsg_SetCacheCapacities,
                     size_t ,
                     size_t ,
                     size_t )

IPC_MESSAGE_CONTROL0(ViewMsg_ClearCache)

IPC_MESSAGE_CONTROL0(ViewMsg_PurgeMemory)

IPC_MESSAGE_ROUTED0(ViewMsg_CaptureSnapshot)

IPC_MESSAGE_CONTROL1(ViewMsg_VisitedLink_NewTable, base::SharedMemoryHandle)

IPC_MESSAGE_CONTROL1(ViewMsg_VisitedLink_Add, std::vector<uint64>)

IPC_MESSAGE_CONTROL0(ViewMsg_VisitedLink_Reset)

IPC_MESSAGE_ROUTED2(ViewMsg_SetContentSettingsForLoadingURL,
                    GURL ,
                    ContentSettings )

IPC_MESSAGE_CONTROL2(ViewMsg_SetContentSettingsForCurrentURL,
                     GURL ,
                     ContentSettings )

IPC_MESSAGE_ROUTED0(ViewMsg_LoadBlockedPlugins)

IPC_MESSAGE_ROUTED0(ViewMsg_EnableViewSourceMode)

IPC_MESSAGE_ROUTED1(ViewMsg_GetAllSavableResourceLinksForCurrentPage,
                    GURL )

IPC_MESSAGE_ROUTED3(ViewMsg_GetSerializedHtmlDataForCurrentPageWithLocalLinks,
                    std::vector<GURL> ,
                    std::vector<FilePath> ,
                    FilePath )

IPC_MESSAGE_CONTROL0(ViewMsg_GetCacheResourceStats)

IPC_MESSAGE_CONTROL1(ViewMsg_GetRendererHistograms,
                     int )

#if defined(USE_TCMALLOC)
IPC_MESSAGE_CONTROL0(ViewMsg_GetRendererTcmalloc)
#endif

IPC_MESSAGE_CONTROL0(ViewMsg_GetV8HeapStats)

IPC_MESSAGE_ROUTED3(ViewMsg_HandleMessageFromExternalHost,
                    std::string ,
                    std::string ,
                    std::string )

IPC_MESSAGE_ROUTED4(ViewMsg_SearchBoxChange,
                    string16 ,
                    bool ,
                    int ,
                    int )
IPC_MESSAGE_ROUTED2(ViewMsg_SearchBoxSubmit,
                    string16 ,
                    bool )
IPC_MESSAGE_ROUTED0(ViewMsg_SearchBoxCancel)
IPC_MESSAGE_ROUTED1(ViewMsg_SearchBoxResize,
                    gfx::Rect )
IPC_MESSAGE_ROUTED4(ViewMsg_DetermineIfPageSupportsInstant,
                    string16 ,
                    bool ,
                    int ,
                    int )

IPC_MESSAGE_ROUTED1(ViewMsg_UpdateBrowserWindowId,
                    int )

IPC_MESSAGE_ROUTED1(ViewMsg_NotifyRenderViewType,
                    ViewType::Type )

IPC_MESSAGE_ROUTED4(ViewMsg_TranslatePage,
                    int ,
                    std::string, 
                    std::string, 
                    std::string 
)

IPC_MESSAGE_ROUTED1(ViewMsg_RevertTranslation,
                    int )

IPC_MESSAGE_CONTROL1(ViewMsg_SetIsIncognitoProcess,
                     bool )


IPC_MESSAGE_ROUTED3(ViewHostMsg_PageContents,
                    GURL         ,
                    int32        ,
                    string16     )

IPC_MESSAGE_ROUTED2(ViewHostMsg_TranslateLanguageDetermined,
                    std::string  ,
                    bool         )

IPC_MESSAGE_CONTROL1(ViewHostMsg_UpdatedCacheStats,
                     WebKit::WebCache::UsageStats )

IPC_MESSAGE_ROUTED2(ViewHostMsg_ContentBlocked,
                    ContentSettingsType, 
                    std::string )

IPC_MESSAGE_ROUTED3(ViewHostMsg_Thumbnail,
                    GURL ,
                    ThumbnailScore ,
                    SkBitmap )

IPC_MESSAGE_ROUTED1(ViewHostMsg_Snapshot,
                    SkBitmap )

IPC_MESSAGE_ROUTED2(ViewHostMsg_DomOperationResponse,
                    std::string  ,
                    int  )

IPC_MESSAGE_ROUTED3(ViewHostMsg_ForwardMessageToExternalHost,
                    std::string  ,
                    std::string  ,
                    std::string  )

IPC_SYNC_MESSAGE_CONTROL2_3(ViewHostMsg_LaunchNaCl,
                            std::wstring ,
                            int ,
                            std::vector<nacl::FileDescriptor>
                                ,
                            base::ProcessHandle ,
                            base::ProcessId )

IPC_MESSAGE_ROUTED3(ViewHostMsg_PageHasOSDD,
                    int32 ,
                    GURL ,
                    search_provider::OSDDType)

IPC_SYNC_MESSAGE_ROUTED2_1(ViewHostMsg_GetSearchProviderInstallState,
                           GURL ,
                           GURL ,
                           search_provider::InstallState )

IPC_MESSAGE_CONTROL1(ViewHostMsg_UserMetricsRecordAction,
                     std::string )

IPC_MESSAGE_CONTROL2(ViewHostMsg_RendererHistograms,
                     int, 
                     std::vector<std::string>)

#if defined USE_TCMALLOC
IPC_MESSAGE_CONTROL2(ViewHostMsg_RendererTcmalloc,
                     int          ,
                     std::string  )
#endif

IPC_MESSAGE_CONTROL2(ViewHostMsg_V8HeapStats,
                     int ,
                     int )

IPC_MESSAGE_CONTROL1(ViewHostMsg_DnsPrefetch,
                     std::vector<std::string> )

IPC_SYNC_MESSAGE_ROUTED0_1(ViewHostMsg_GetOutdatedPluginsPolicy,
                           ContentSetting   )

IPC_MESSAGE_ROUTED2(ViewHostMsg_BlockedOutdatedPlugin,
                    string16, 
                    GURL      )

IPC_MESSAGE_ROUTED3(ViewHostMsg_SendCurrentPageAllSavableResourceLinks,
                    std::vector<GURL> ,
                    std::vector<GURL> ,
                    std::vector<GURL> )

IPC_MESSAGE_ROUTED3(ViewHostMsg_SendSerializedHtmlData,
                    GURL ,
                    std::string ,
                    int32 )

IPC_MESSAGE_CONTROL1(ViewHostMsg_ResourceTypeStats,
                     WebKit::WebCache::ResourceTypeStats)

IPC_MESSAGE_ROUTED3(ViewHostMsg_CommandStateChanged,
                    int ,
                    bool ,
                    int )


IPC_MESSAGE_ROUTED1(ViewHostMsg_PageLanguageDetermined,
                    std::string )

IPC_MESSAGE_ROUTED4(ViewHostMsg_PageTranslated,
                    int,                  
                    std::string           ,
                    std::string           ,
                    TranslateErrors::Type )


IPC_MESSAGE_ROUTED3(ViewHostMsg_SetSuggestions,
                    int32 ,
                    std::vector<std::string> ,
                    InstantCompleteBehavior)

IPC_MESSAGE_ROUTED2(ViewHostMsg_InstantSupportDetermined,
                    int32 ,
                    bool  )


IPC_MESSAGE_ROUTED2(ViewMsg_JavaScriptStressTestControl,
                    int ,
                    int )
