// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.





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

IPC_SYNC_MESSAGE_CONTROL3_1(AutomationMsg_DEPRECATED_SetCookie,
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

IPC_SYNC_MESSAGE_CONTROL3_1(AutomationMsg_DomOperation,
                            int ,
                            std::wstring ,
                            std::wstring ,
                            std::string 
)

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_DEPRECATED_ShelfVisibility,
                            int ,
                            bool )

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

IPC_SYNC_MESSAGE_CONTROL4_1(AutomationMsg_DEPRECATED_WindowDrag,
                           int,
                           std::vector<gfx::Point>,
                           int,
                           bool,
                           bool)

IPC_MESSAGE_CONTROL1(AutomationMsg_InitialNewTabUILoadComplete,
                    int )

IPC_MESSAGE_CONTROL1(AutomationMsg_DEPRECATED_SetFilteredInet,
                     bool )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_DEPRECATED_DownloadDirectory,
                            int ,
                            base::FilePath )

IPC_SYNC_MESSAGE_CONTROL1_0(AutomationMsg_OpenNewBrowserWindow,
                            bool  )

IPC_SYNC_MESSAGE_CONTROL0_1(AutomationMsg_DEPRECATED_ActiveWindow,
                            int)

IPC_SYNC_MESSAGE_CONTROL1_2(AutomationMsg_WindowForBrowser,
                            int ,
                            bool ,
                            int )

IPC_MESSAGE_CONTROL3(AutomationMsg_DEPRECATED_WindowKeyPress,
                     int,
                     int,
                     int)
#if defined(OS_WIN)
IPC_SYNC_MESSAGE_CONTROL1_4(AutomationMsg_CreateExternalTab,
                            ExternalTabSettings  ,
                            HWND  ,
                            HWND  ,
                            int  ,
                            int  )
#endif  
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


IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_CloseTab,
                            int,
                            bool,
                            bool)

IPC_SYNC_MESSAGE_CONTROL1_2(AutomationMsg_CloseBrowser,
                            int,
                            bool,
                            bool)

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

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_DEPRECATED_WaitForTabToBeRestored,
                            int, bool)

IPC_MESSAGE_ROUTED1(AutomationMsg_DidNavigate,
                    NavigationInfo)


IPC_SYNC_MESSAGE_CONTROL1_4(AutomationMsg_DEPRECATED_GetSecurityState,
                            int,
                            bool,
                            content::SecurityStyle,
                            net::CertStatus,
                            int)

IPC_SYNC_MESSAGE_CONTROL1_2(AutomationMsg_DEPRECATED_GetPageType,
                            int,
                            bool,
                            content::PageType)

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_DEPRECATED_ActionOnSSLBlockingPage,
                            int,
                            bool,
                            AutomationMsg_NavigationResponseValues)

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_BringBrowserToFront,
                            int,
                            bool)

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_DEPRECATED_IsMenuCommandEnabled,
                            int,
                            int,
                            bool)

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_Reload,
                            int,
                            AutomationMsg_NavigationResponseValues)

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_WindowExecuteCommandAsync,
                            int ,
                            int ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_WindowExecuteCommand,
                            int ,
                            int ,
                            bool )


IPC_MESSAGE_CONTROL1(AutomationMsg_DEPRECATED_OpenFindInPage,
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

IPC_SYNC_MESSAGE_CONTROL1_3(AutomationMsg_BookmarkBarVisibility,
                            int ,
                            bool, 
                            bool,  bool )

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_OverrideEncoding,
                            int ,
                            std::string ,
                            bool )

IPC_MESSAGE_ROUTED2(AutomationMsg_NavigationFailed,
                    int,
                    GURL)

#if defined(OS_WIN)
IPC_MESSAGE_CONTROL2(AutomationMsg_TabReposition,
                     int ,
                     Reposition_Params )
#endif  

IPC_MESSAGE_ROUTED1(AutomationMsg_TabLoaded,
                    GURL)

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_TabIndex,
                            int,
                            int)

IPC_SYNC_MESSAGE_CONTROL0_1(AutomationMsg_DEPRECATED_FindTabbedBrowserWindow,
                            int)

IPC_SYNC_MESSAGE_CONTROL0_1(AutomationMsg_NormalBrowserWindowCount,
                            int)

IPC_MESSAGE_CONTROL1(AutomationMsg_SetProxyConfig,
                     std::string )

IPC_SYNC_MESSAGE_CONTROL2_0(AutomationMsg_DEPRECATED_SetShelfVisibility,
                            int ,
                            bool )

#if defined(OS_WIN)
IPC_MESSAGE_ROUTED3(AutomationMsg_ForwardContextMenuToExternalHost,
                    ContextMenuModel ,
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

IPC_MESSAGE_ROUTED2(AutomationMsg_DEPRECATED_SetCookieAsync,
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

IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_GoBackBlockUntilNavigationsComplete,
                            int,
                            int,
                            AutomationMsg_NavigationResponseValues)

IPC_SYNC_MESSAGE_CONTROL2_1(
    AutomationMsg_GoForwardBlockUntilNavigationsComplete,
    int,
    int,
    AutomationMsg_NavigationResponseValues)

IPC_MESSAGE_ROUTED1(AutomationMsg_AttachExternalTab,
                    AttachExternalTabParams)
#if defined(OS_WIN)
IPC_SYNC_MESSAGE_CONTROL3_4(AutomationMsg_ConnectExternalTab,
                            uint64 ,
                            bool   ,
                            HWND  ,
                            HWND  ,
                            HWND  ,
                            int  ,
                            int  )
#endif  
IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_TerminateSession,
                            int,
                            bool)

IPC_MESSAGE_CONTROL2(AutomationMsg_SetPageFontSize,
                     int ,
                     int )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_GetMetricEventDuration,
                            std::string ,
                            int )

IPC_MESSAGE_ROUTED1(AutomationMsg_RequestGoToHistoryEntryOffset,
                    int)   

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_Type,
                            int,
                            int)

IPC_SYNC_MESSAGE_CONTROL2_0(AutomationMsg_OpenNewBrowserWindowOfType,
                            int   ,
                            bool   )

IPC_MESSAGE_CONTROL2(AutomationMsg_DEPRECATED_WindowMouseMove,
                     int,
                     gfx::Point)

IPC_MESSAGE_ROUTED1(AutomationMsg_DownloadRequestInHost,
                    int )

IPC_MESSAGE_CONTROL1(AutomationMsg_SaveAsAsync,
                     int )

#if defined(OS_WIN)
IPC_MESSAGE_CONTROL1(AutomationMsg_BrowserMove,
                     int )
#endif

IPC_MESSAGE_ROUTED2(AutomationMsg_DEPRECATED_GetCookiesFromHost,
                    GURL ,
                    int )

IPC_MESSAGE_CONTROL5(AutomationMsg_DEPRECATED_GetCookiesHostResponse,
                     int ,
                     bool ,
                     GURL ,
                     std::string ,
                     int )

IPC_SYNC_MESSAGE_CONTROL1_2(AutomationMsg_DEPRECATED_GetBookmarksAsJSON,
                            int ,
                            std::string ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_WaitForBookmarkModelToLoad,
                            int ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL4_1(AutomationMsg_DEPRECATED_AddBookmarkGroup,
                            int ,
                            int64 ,
                            int ,
                            std::wstring ,
                            bool )
IPC_SYNC_MESSAGE_CONTROL5_1(AutomationMsg_DEPRECATED_AddBookmarkURL,
                            int ,
                            int64 ,
                            int ,
                            std::wstring ,
                            GURL ,
                            bool )
IPC_SYNC_MESSAGE_CONTROL4_1(AutomationMsg_DEPRECATED_ReparentBookmark,
                            int ,
                            int64 ,
                            int64 ,
                            int ,
                            bool )
IPC_SYNC_MESSAGE_CONTROL3_1(AutomationMsg_DEPRECATED_SetBookmarkTitle,
                            int ,
                            int64 ,
                            std::wstring ,
                            bool )
IPC_SYNC_MESSAGE_CONTROL3_1(AutomationMsg_DEPRECATED_SetBookmarkURL,
                            int ,
                            int64 ,
                            GURL ,
                            bool )
IPC_SYNC_MESSAGE_CONTROL2_1(AutomationMsg_DEPRECATED_RemoveBookmark,
                            int ,
                            int64 ,
                            bool )

IPC_MESSAGE_CONTROL1(AutomationMsg_RemoveBrowsingData,
                     int)

IPC_SYNC_MESSAGE_CONTROL2_2(AutomationMsg_SendJSONRequestWithBrowserHandle,
                            int ,
                            std::string ,
                            std::string ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL0_0(AutomationMsg_DEPRECATED_ResetToDefaultTheme)

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

IPC_MESSAGE_CONTROL3(AutomationMsg_JavaScriptStressTestControl,
                     int ,
                     int ,
                     int )

IPC_SYNC_MESSAGE_CONTROL0_0(AutomationMsg_WaitForProcessLauncherThreadToGoIdle)

IPC_MESSAGE_ROUTED1(AutomationMsg_MoveWindow,
                    gfx::Rect )

IPC_SYNC_MESSAGE_CONTROL1_1(AutomationMsg_BeginTracing,
                            std::string ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL0_2(AutomationMsg_EndTracing,
                            base::FilePath ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL0_1(AutomationMsg_GetMachPortCount,
                            int )

IPC_SYNC_MESSAGE_CONTROL2_2(AutomationMsg_SendJSONRequest,
                            int ,
                            std::string ,
                            std::string ,
                            bool )
