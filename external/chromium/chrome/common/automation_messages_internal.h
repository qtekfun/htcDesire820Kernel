// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <string>
#include <vector>

#include "base/string16.h"
#include "chrome/common/content_settings.h"
#include "chrome/test/automation/autocomplete_edit_proxy.h"
#include "content/common/navigation_types.h"
#include "googleurl/src/gurl.h"
#include "ipc/ipc_message_macros.h"
#include "net/url_request/url_request_status.h"
#include "ui/gfx/rect.h"
#include "webkit/glue/window_open_disposition.h"




#define IPC_MESSAGE_START AutomationMsgStart

IPC_MESSAGE_CONTROL1(AutomationMsg_Hello,
                     std::string)

IPC_MESSAGE_CONTROL0(AutomationMsg_InitialLoadsComplete)

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_AppendTab,
                            int,
                            GURL,
                            int)

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_ActiveTabIndex,
                            int,
                            int)

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_ActivateTab,
                            int,
                            int,
                            int)

IPC_SYNC_MESSAGE_CONTROL2_2(AutomationMsg_GetCookies,
                            GURL,
                            int,
                            int,
                            std::string)

IPC_SYNC_MESSAGE_CONTROL3_1(AutomationMsg_SetCookie,
                            GURL,
                            std::string,
                            int,
                            int)

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_NavigationAsync,
                            int ,
                            GURL,
                            bool )

IPC_SYNC_MESSAGE_CONTROL0_1(AutomationMsg_BrowserWindowCount,
                            int)

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_BrowserWindow,
                            int,
                            int)

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_TabCount,
                            int,
                            int)

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_Tab,
                            int,
                            int,
                            int)

IPC_SYNC_MESSAGE_CONTROL1_2(AutomationMsg_TabTitle,
                            int,
                            int,
                            std::wstring)

IPC_SYNC_MESSAGE_CONTROL1_2(AutomationMsg_TabURL,
                            int ,
                            bool ,
                            GURL)

IPC_MESSAGE_CONTROL1(AutomationMsg_InvalidateHandle,
                     int)

IPC_MESSAGE_CONTROL1(AutomationMsg_HandleUnused,
                     int)

IPC_SYNC_MESSAGE_CONTROL3_1(AutomationMsg_SetAuth,
                            int ,
                            std::wstring ,
                            std::wstring ,
                            AutomationMsg_NavigationResponseValues )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_CancelAuth,
                            int ,
                            AutomationMsg_NavigationResponseValues )

IPC_SYNC_MESSAGE_CONTROL2_2(AutomationMsg_RedirectsFrom,
                            int ,
                            GURL ,
                            bool ,
                            std::vector<GURL> )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_NeedsAuth,
                            int ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL3_1(AutomationMsg_DomOperation,
                            int ,
                            std::wstring ,
                            std::wstring ,
                            std::string 
)

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_ShelfVisibility,
                            int ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_ConstrainedWindowCount,
                            int ,
                            int )

IPC_SYNC_MESSAGE_CONTROL3_2(AutomationMsg_WindowViewBounds,
                            int,
                            int,
                            bool,
                            bool,
                            gfx::Rect)

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_SetWindowBounds,
                            int,
                            gfx::Rect,
                            bool)

IPC_SYNC_MESSAGE_CONTROL4_1(AutomationMsg_WindowDrag,
                           int,
                           std::vector<gfx::Point>,
                           int,
                           bool,
                           bool)

IPC_MESSAGE_CONTROL1(AutomationMsg_InitialNewTabUILoadComplete,
                    int )

IPC_SYNC_MESSAGE_CONTROL3_1(AutomationMsg_InspectElement,
                            int, 
                            int, 
                            int  ,
                            int)

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_TabProcessID,
                            int ,
                            int )

IPC_MESSAGE_CONTROL1(AutomationMsg_SetFilteredInet,
                     bool )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_DownloadDirectory,
                            int ,
                            FilePath )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_GetFocusedViewID,
                            int ,
                            int )

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_SetWindowVisible,
                            int ,
                            bool ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL1_2(AutomationMsg_IsWindowActive,
                            int ,
                            bool ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL1_0(AutomationMsg_ActivateWindow,
                            int )

IPC_SYNC_MESSAGE_CONTROL1_0(AutomationMsg_OpenNewBrowserWindow,
                            bool  )

IPC_SYNC_MESSAGE_CONTROL0_1(AutomationMsg_ActiveWindow,
                            int)

IPC_SYNC_MESSAGE_CONTROL1_2(AutomationMsg_BrowserForWindow,
                            int ,
                            bool ,
                            int )

IPC_SYNC_MESSAGE_CONTROL1_2(AutomationMsg_WindowForBrowser,
                            int ,
                            bool ,
                            int )

IPC_SYNC_MESSAGE_CONTROL1_2(AutomationMsg_AutocompleteEditForBrowser,
                            int ,
                            bool ,
                            int )

IPC_MESSAGE_CONTROL3(AutomationMsg_WindowClick,
                     int,
                     gfx::Point,
                     int)

IPC_MESSAGE_CONTROL3(AutomationMsg_WindowKeyPress,
                     int,
                     int,
                     int)

IPC_SYNC_MESSAGE_CONTROL1_4(AutomationMsg_CreateExternalTab,
                            ExternalTabSettings  ,
                            gfx::NativeWindow  ,
                            gfx::NativeWindow  ,
                            int  ,
                            int  )

IPC_SYNC_MESSAGE_CONTROL3_1(AutomationMsg_NavigateInExternalTab,
                            int,
                            GURL,
                            GURL,
                            AutomationMsg_NavigationResponseValues)

IPC_MESSAGE_ROUTED2(AutomationMsg_NavigationStateChanged,
                    int,  
                    NavigationInfo)  

IPC_MESSAGE_ROUTED1(AutomationMsg_UpdateTargetUrl,
                    std::wstring)

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_ShowInterstitialPage,
                            int,
                            std::string,
                            AutomationMsg_NavigationResponseValues)

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_HideInterstitialPage,
                            int,
                            bool)

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_CloseTab,
                            int,
                            bool,
                            bool)

IPC_SYNC_MESSAGE_CONTROL1_2(AutomationMsg_CloseBrowser,
                            int,
                            bool,
                            bool)

IPC_MESSAGE_CONTROL1(AutomationMsg_CloseBrowserRequestAsync,
                     int)

#if defined(OS_WIN)
IPC_MESSAGE_ROUTED1(AutomationMsg_HandleAccelerator,
                    MSG)

IPC_MESSAGE_CONTROL2(AutomationMsg_ProcessUnhandledAccelerator,
                     int,
                     MSG)
#endif  

IPC_MESSAGE_ROUTED1(AutomationMsg_TabbedOut,
                    bool)

IPC_MESSAGE_CONTROL3(AutomationMsg_SetInitialFocus,
                     int,
                     bool,
                     bool)

IPC_MESSAGE_ROUTED3(AutomationMsg_OpenURL,
                    GURL,
                    GURL,
                    int)

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_WaitForTabToBeRestored,
                            int, bool)

IPC_MESSAGE_ROUTED1(AutomationMsg_DidNavigate,
                    NavigationInfo)


IPC_SYNC_MESSAGE_CONTROL1_4(AutomationMsg_GetSecurityState,
                            int,
                            bool,
                            SecurityStyle,
                            int,
                            int)

IPC_SYNC_MESSAGE_CONTROL1_2(AutomationMsg_GetPageType,
                            int,
                            bool,
                            PageType)

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_ActionOnSSLBlockingPage,
                            int,
                            bool,
                            AutomationMsg_NavigationResponseValues)

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_BringBrowserToFront,
                            int,
                            bool)

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_IsMenuCommandEnabled,
                            int,
                            int,
                            bool)

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_PrintNow,
                            int,
                            bool)

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_Reload,
                            int,
                            AutomationMsg_NavigationResponseValues)

IPC_SYNC_MESSAGE_CONTROL0_1(AutomationMsg_LastActiveBrowserWindow,
                            int)

IPC_SYNC_MESSAGE_CONTROL4_1(AutomationMsg_SavePage,
                            int,
                            FilePath,
                            FilePath,
                            int,
                            bool)

IPC_SYNC_MESSAGE_CONTROL1_2(AutomationMsg_AutocompleteEditGetText,
                            int ,
                            bool ,
                            string16 )

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_AutocompleteEditSetText,
                            int ,
                            string16 ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL1_2( \
    AutomationMsg_AutocompleteEditIsQueryInProgress,
    int ,
    bool ,
    bool )

IPC_SYNC_MESSAGE_CONTROL1_2(AutomationMsg_AutocompleteEditGetMatches,
                            int ,
                            bool ,
                            std::vector<AutocompleteMatchData> )

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_WindowExecuteCommandAsync,
                            int ,
                            int ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_WindowExecuteCommand,
                            int ,
                            int ,
                            bool )


IPC_MESSAGE_CONTROL1(AutomationMsg_OpenFindInPage,
                     int )

IPC_MESSAGE_CONTROL4(AutomationMsg_HandleMessageFromExternalHost,
                     int ,
                     std::string ,
                     std::string ,
                     std::string )

IPC_MESSAGE_ROUTED3(AutomationMsg_ForwardMessageToExternalHost,
                    std::string ,
                    std::string ,
                    std::string )

IPC_SYNC_MESSAGE_CONTROL2_2(AutomationMsg_Find,
                            int ,
                            AutomationMsg_Find_Params ,
                            int ,
                            int )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_FindWindowVisibility,
                            int ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL1_2(AutomationMsg_FindWindowLocation,
                            int ,
                            int ,
                            int )

IPC_SYNC_MESSAGE_CONTROL1_2(AutomationMsg_BookmarkBarVisibility,
                            int ,
                            bool, 
                            bool  )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_GetInfoBarCount,
                            int ,
                            size_t )

IPC_SYNC_MESSAGE_CONTROL3_1(AutomationMsg_ClickInfoBarAccept,
                            int ,
                            size_t ,
                            bool ,
                            
                            AutomationMsg_NavigationResponseValues)

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_GetLastNavigationTime,
                            int ,
                            int64 )

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_WaitForNavigation,
                            int ,
                            int64 ,
                            
                            AutomationMsg_NavigationResponseValues)

IPC_SYNC_MESSAGE_CONTROL3_1(AutomationMsg_SetIntPreference,
                            int ,
                            std::string ,
                            int ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL0_2(AutomationMsg_ShowingAppModalDialog,
                            bool ,
                            int )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_ClickAppModalDialogButton,
                            int ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL3_1(AutomationMsg_SetStringPreference,
                            int ,
                            std::string ,
                            std::string ,
                            bool)

IPC_SYNC_MESSAGE_CONTROL2_2(AutomationMsg_GetBooleanPreference,
                           int ,
                           std::string ,
                           bool ,
                           bool )

IPC_SYNC_MESSAGE_CONTROL3_1(AutomationMsg_SetBooleanPreference,
                            int ,
                            std::string ,
                            bool ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_GetPageCurrentEncoding,
                            int ,
                            std::string )

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_OverrideEncoding,
                            int ,
                            std::string ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL1_0(AutomationMsg_SavePackageShouldPromptUser,
                            bool )

IPC_MESSAGE_ROUTED2(AutomationMsg_NavigationFailed,
                    int,
                    GURL)

#if defined(OS_WIN)
IPC_MESSAGE_CONTROL2(AutomationMsg_TabReposition,
                     int ,
                     Reposition_Params )
#endif  

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_WindowTitle,
                            int ,
                            string16  )

IPC_MESSAGE_ROUTED1(AutomationMsg_TabLoaded,
                    GURL)

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_TabIndex,
                            int,
                            int)

IPC_SYNC_MESSAGE_CONTROL0_1(AutomationMsg_FindNormalBrowserWindow,
                            int)

IPC_SYNC_MESSAGE_CONTROL0_1(AutomationMsg_NormalBrowserWindowCount,
                            int)

IPC_MESSAGE_CONTROL2(AutomationMsg_DeprecatedMessageOne,
                     
                     int,
                     
                     
                     
                     
                     int)

IPC_MESSAGE_CONTROL1(AutomationMsg_SetProxyConfig,
                     std::string )

IPC_SYNC_MESSAGE_CONTROL2_0(AutomationMsg_SetShelfVisibility,
                            int ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_BlockedPopupCount,
                            int ,
                            int )

IPC_SYNC_MESSAGE_CONTROL0_1(AutomationMsg_GetBrowserLocale,
                            string16 )

#if defined(OS_WIN)
IPC_MESSAGE_ROUTED3(AutomationMsg_ForwardContextMenuToExternalHost,
                    HANDLE ,
                    int    ,
                    MiniContextMenuParams )

IPC_MESSAGE_CONTROL2(AutomationMsg_ForwardContextMenuCommandToChrome,
                     int ,
                     int )
#endif  

IPC_MESSAGE_ROUTED2(AutomationMsg_RequestStart,
                    int ,
                    AutomationURLRequest )

IPC_MESSAGE_ROUTED2(AutomationMsg_RequestRead,
                    int ,
                    int )

IPC_MESSAGE_ROUTED2(AutomationMsg_RequestStarted,
                    int ,
                    AutomationURLResponse )

IPC_MESSAGE_ROUTED2(AutomationMsg_RequestData,
                    int ,
                    std::string )

IPC_MESSAGE_ROUTED2(AutomationMsg_RequestEnd,
                    int ,
                    net::URLRequestStatus )

IPC_MESSAGE_CONTROL1(AutomationMsg_PrintAsync,
                     int )

IPC_MESSAGE_ROUTED2(AutomationMsg_SetCookieAsync,
                    GURL ,
                    std::string )

IPC_MESSAGE_CONTROL1(AutomationMsg_SelectAll,
                    int )

IPC_MESSAGE_CONTROL1(AutomationMsg_Cut,
                     int )

IPC_MESSAGE_CONTROL1(AutomationMsg_Copy,
                     int )

IPC_MESSAGE_CONTROL1(AutomationMsg_Paste,
                     int )

IPC_MESSAGE_CONTROL1(AutomationMsg_ReloadAsync,
                     int )

IPC_MESSAGE_CONTROL1(AutomationMsg_StopAsync,
                     int )

IPC_SYNC_MESSAGE_CONTROL0_1(AutomationMsg_GetFilteredInetHitCount,
                            int )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_IsFullscreen,
                            int ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_IsFullscreenBubbleVisible,
                            int ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL3_1(
    AutomationMsg_NavigateToURLBlockUntilNavigationsComplete,
    int,
    GURL,
    int,
    AutomationMsg_NavigationResponseValues)

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_NavigateExternalTabAtIndex,
                            int,
                            int,
                            AutomationMsg_NavigationResponseValues)

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_WaitForBrowserWindowCountToBecome,
                           int,
                           bool)

IPC_SYNC_MESSAGE_CONTROL0_1(AutomationMsg_WaitForAppModalDialogToBeShown,
                            bool)

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_GoBackBlockUntilNavigationsComplete,
                            int,
                            int,
                            AutomationMsg_NavigationResponseValues)

IPC_SYNC_MESSAGE_CONTROL2_1(
    AutomationMsg_GoForwardBlockUntilNavigationsComplete,
    int,
    int,
    AutomationMsg_NavigationResponseValues)

IPC_MESSAGE_CONTROL1(AutomationMsg_RecordHistograms,
                     std::vector<std::string> )

IPC_MESSAGE_ROUTED1(AutomationMsg_AttachExternalTab,
                    AttachExternalTabParams)

IPC_SYNC_MESSAGE_CONTROL3_4(AutomationMsg_ConnectExternalTab,
                            uint64 ,
                            bool   ,
                            gfx::NativeWindow  ,
                            gfx::NativeWindow  ,
                            gfx::NativeWindow  ,
                            int  ,
                            int  )

IPC_SYNC_MESSAGE_CONTROL1_2(AutomationMsg_GetWindowBounds,
                            int,
                            gfx::Rect,
                            bool)

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_TerminateSession,
                            int,
                            bool)

IPC_SYNC_MESSAGE_CONTROL1_2(AutomationMsg_IsWindowMaximized,
                            int,
                            bool,
                            bool)

IPC_MESSAGE_CONTROL2(AutomationMsg_SetPageFontSize,
                     int ,
                     int )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_GetMetricEventDuration,
                            std::string ,
                            int )

IPC_MESSAGE_ROUTED1(AutomationMsg_RequestGoToHistoryEntryOffset,
                    int)   

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_InstallExtension,
                            FilePath ,
                            AutomationMsg_ExtensionResponseValues)

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_DeprecatedMessageTwo,
                            int,
                            int)

IPC_SYNC_MESSAGE_CONTROL0_1(AutomationMsg_DeprecatedMessageThree,
                            int)

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_Type,
                            int,
                            int)

IPC_SYNC_MESSAGE_CONTROL2_0(AutomationMsg_OpenNewBrowserWindowOfType,
                            int   ,
                            bool   )

IPC_MESSAGE_CONTROL2(AutomationMsg_WindowMouseMove,
                     int,
                     gfx::Point)

IPC_MESSAGE_ROUTED1(AutomationMsg_DownloadRequestInHost,
                    int )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_ShutdownSessionService,
                            int   ,
                            bool  )

IPC_MESSAGE_CONTROL1(AutomationMsg_SaveAsAsync,
                     int )

#if defined(OS_WIN)
IPC_MESSAGE_CONTROL1(AutomationMsg_BrowserMove,
                     int )
#endif

IPC_MESSAGE_ROUTED2(AutomationMsg_GetCookiesFromHost,
                    GURL ,
                    int )

IPC_MESSAGE_CONTROL5(AutomationMsg_GetCookiesHostResponse,
                     int ,
                     bool ,
                     GURL ,
                     std::string ,
                     int )

IPC_SYNC_MESSAGE_CONTROL4_1(AutomationMsg_SetContentSetting,
                            int ,
                            std::string ,
                            ContentSettingsType ,
                            ContentSetting ,
                            bool )

#if defined(OS_CHROMEOS)
IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_LoginWithUserAndPass,
                           std::string ,
                           std::string ,
                           bool )
#endif

IPC_SYNC_MESSAGE_CONTROL1_2(AutomationMsg_GetBookmarksAsJSON,
                            int ,
                            std::string ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_WaitForBookmarkModelToLoad,
                            int ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL4_1(AutomationMsg_AddBookmarkGroup,
                            int ,
                            int64 ,
                            int ,
                            std::wstring ,
                            bool )
IPC_SYNC_MESSAGE_CONTROL5_1(AutomationMsg_AddBookmarkURL,
                            int ,
                            int64 ,
                            int ,
                            std::wstring ,
                            GURL ,
                            bool )
IPC_SYNC_MESSAGE_CONTROL4_1(AutomationMsg_ReparentBookmark,
                            int ,
                            int64 ,
                            int64 ,
                            int ,
                            bool )
IPC_SYNC_MESSAGE_CONTROL3_1(AutomationMsg_SetBookmarkTitle,
                            int ,
                            int64 ,
                            std::wstring ,
                            bool )
IPC_SYNC_MESSAGE_CONTROL3_1(AutomationMsg_SetBookmarkURL,
                            int ,
                            int64 ,
                            GURL ,
                            bool )
IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_RemoveBookmark,
                            int ,
                            int64 ,
                            bool )

IPC_MESSAGE_CONTROL1(AutomationMsg_RemoveBrowsingData,
                     int)

IPC_SYNC_MESSAGE_CONTROL2_2(AutomationMsg_WaitForFocusedViewIDToChange,
                            int ,
                            int ,
                            bool ,
                            int )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_StartTrackingPopupMenus,
                            int ,
                            bool )
IPC_SYNC_MESSAGE_CONTROL0_1(AutomationMsg_WaitForPopupMenuToOpen,
                            bool )

IPC_SYNC_MESSAGE_CONTROL2_2(AutomationMsg_SendJSONRequest,
                            int ,
                            std::string ,
                            std::string ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_InstallExtensionAndGetHandle,
                            FilePath     ,
                            bool         ,
                            int          )

IPC_SYNC_MESSAGE_CONTROL0_2(AutomationMsg_WaitForExtensionTestResult,
                            bool         ,
                            std::string  )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_UninstallExtension,
                            int   ,
                            bool  )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_EnableExtension,
                           int   ,
                           bool  )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_DisableExtension,
                            int   ,
                            bool  )

IPC_SYNC_MESSAGE_CONTROL2_1(
    AutomationMsg_ExecuteExtensionActionInActiveTabAsync,
    int   ,
    int   ,
    bool  )

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_MoveExtensionBrowserAction,
                            int   ,
                            int   ,
                            bool  )

IPC_SYNC_MESSAGE_CONTROL2_2(AutomationMsg_GetExtensionProperty,
    int                              ,
    AutomationMsg_ExtensionProperty  ,
    bool                             ,
    std::string                      )

IPC_SYNC_MESSAGE_CONTROL0_0(AutomationMsg_ResetToDefaultTheme)

IPC_SYNC_MESSAGE_CONTROL3_1(AutomationMsg_NavigationAsyncWithDisposition,
                            int ,
                            GURL,
                            WindowOpenDisposition,
                            bool )


IPC_SYNC_MESSAGE_CONTROL3_1(AutomationMsg_DeleteCookie,
                            GURL,
                            std::string,
                            int ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_ShowCollectedCookiesDialog,
                            int ,
                            bool )

IPC_MESSAGE_ROUTED0(AutomationMsg_CloseExternalTab)

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_RunUnloadHandlers,
                            int,
                            bool)

IPC_MESSAGE_CONTROL2(AutomationMsg_SetZoomLevel,
                     int,
                     int)

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_WaitForTabCountToBecome,
                            int ,
                            int ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_WaitForInfoBarCount,
                            int ,
                            size_t ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_WaitForAutocompleteEditFocus,
                            int ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_LoadBlockedPlugins,
                            int ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_CaptureEntirePageAsPNG_Deprecated,
                            int,
                            FilePath,
                            bool)

IPC_MESSAGE_CONTROL3(AutomationMsg_JavaScriptStressTestControl,
                     int ,
                     int ,
                     int )

IPC_SYNC_MESSAGE_CONTROL0_0(AutomationMsg_WaitForProcessLauncherThreadToGoIdle)

IPC_SYNC_MESSAGE_CONTROL1_2(AutomationMsg_GetParentBrowserOfTab,
                            int ,
                            int ,
                            bool )

IPC_MESSAGE_ROUTED1(AutomationMsg_MoveWindow,
                    gfx::Rect )


IPC_MESSAGE_ROUTED2(AutomationMsg_WillPerformClientRedirect,
                    int64 ,
                    double )

IPC_MESSAGE_ROUTED1(AutomationMsg_DidCompleteOrCancelClientRedirect,
                    int64 )


