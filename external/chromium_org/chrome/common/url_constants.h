// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_COMMON_URL_CONSTANTS_H_
#define CHROME_COMMON_URL_CONSTANTS_H_

#include <string>
#include <vector>

#include "build/build_config.h"
#include "content/public/common/url_constants.h"

namespace chrome {

extern const char kAboutComponentsURL[];
extern const char kAboutPluginsURL[];
extern const char kAboutVersionURL[];

extern const char kChromeUIAboutURL[];
extern const char kChromeUIAppsURL[];
extern const char kChromeUIAppListStartPageURL[];
extern const char kChromeUIBookmarksURL[];
extern const char kChromeUICertificateViewerURL[];
extern const char kChromeUIChromeSigninURL[];
extern const char kChromeUIChromeURLsURL[];
extern const char kChromeUICloudPrintResourcesURL[];
extern const char kChromeUIComponentsURL[];
extern const char kChromeUIConflictsURL[];
extern const char kChromeUIConstrainedHTMLTestURL[];
extern const char kChromeUICrashesURL[];
extern const char kChromeUICreditsURL[];
extern const char kChromeUIDevicesURL[];
extern const char kChromeUIDevToolsURL[];
extern const char kChromeUIDownloadsURL[];
extern const char kChromeUIEditSearchEngineDialogURL[];
extern const char kChromeUIExtensionIconURL[];
extern const char kChromeUIExtensionInfoURL[];
extern const char kChromeUIExtensionsFrameURL[];
extern const char kChromeUIExtensionsURL[];
extern const char kChromeUIFaviconURL[];
extern const char kChromeUIFeedbackURL[];
extern const char kChromeUIFlagsURL[];
extern const char kChromeUIFlashURL[];
extern const char kChromeUIHelpFrameURL[];
extern const char kChromeUIHistoryURL[];
extern const char kChromeUIHistoryFrameURL[];
extern const char kChromeUIIdentityInternalsURL[];
extern const char kChromeUIInspectURL[];
extern const char kChromeUIInstantURL[];
extern const char kChromeUIIPCURL[];
extern const char kChromeUIManagedUserPassphrasePageURL[];
extern const char kChromeUIMemoryRedirectURL[];
extern const char kChromeUIMemoryURL[];
extern const char kChromeUIMetroFlowURL[];
extern const char kChromeUINaClURL[];
extern const char kChromeUINetInternalsURL[];
extern const char kChromeUINewProfile[];
extern const char kChromeUINewTabURL[];
extern const char kChromeUIOmniboxURL[];
extern const char kChromeUIPerformanceMonitorURL[];
extern const char kChromeUIPluginsURL[];
extern const char kChromeUIPolicyURL[];
extern const char kChromeUIProfileSigninConfirmationURL[];
extern const char kChromeUIUserManagerURL[];
extern const char kChromeUIPrintURL[];
extern const char kChromeUIQuitURL[];
extern const char kChromeUIRestartURL[];
extern const char kChromeUISessionFaviconURL[];
extern const char kChromeUISettingsURL[];
extern const char kChromeUISettingsFrameURL[];
extern const char kChromeUISuggestionsInternalsURL[];
extern const char kChromeUISSLClientCertificateSelectorURL[];
extern const char kChromeUITaskManagerURL[];
extern const char kChromeUITermsURL[];
extern const char kChromeUIThemeURL[];
extern const char kChromeUIThumbnailURL[];
extern const char kChromeUIThumbnailListURL[];
extern const char kChromeUIUberURL[];
extern const char kChromeUIUberFrameURL[];
extern const char kChromeUIUserActionsURL[];
extern const char kChromeUIVersionURL[];

#if defined(OS_ANDROID)
extern const char kChromeUINativeNewTabURL[];
extern const char kChromeUIWelcomeURL[];
#endif

#if defined(OS_CHROMEOS)
extern const char kChromeUIActivationMessage[];
extern const char kChromeUIBluetoothPairingURL[];
extern const char kChromeUIChargerReplacementURL[];
extern const char kChromeUIChooseMobileNetworkURL[];
extern const char kChromeUIDiagnosticsURL[];
extern const char kChromeUIDiscardsURL[];
extern const char kChromeUIFirstRunURL[];
extern const char kChromeUIIdleLogoutDialogURL[];
extern const char kChromeUIImageBurnerURL[];
extern const char kChromeUIKeyboardOverlayURL[];
extern const char kChromeUILockScreenURL[];
extern const char kChromeUIMediaplayerURL[];
extern const char kChromeUIMobileSetupURL[];
extern const char kChromeUIOobeURL[];
extern const char kChromeUIOSCreditsURL[];
extern const char kChromeUIProxySettingsURL[];
extern const char kChromeUIScreenlockIconURL[];
extern const char kChromeUISimUnlockURL[];
extern const char kChromeUISlideshowURL[];
extern const char kChromeUISlowURL[];
extern const char kChromeUISystemInfoURL[];
extern const char kChromeUITermsOemURL[];
extern const char kChromeUIUserImageURL[];
#endif

#if defined(USE_AURA)
extern const char kChromeUIGestureConfigURL[];
extern const char kChromeUIGestureConfigHost[];
extern const char kChromeUISalsaURL[];
extern const char kChromeUISalsaHost[];
#endif

#if (defined(OS_LINUX) && defined(TOOLKIT_VIEWS)) || defined(USE_AURA)
extern const char kChromeUITabModalConfirmDialogURL[];
#endif

#if defined(ENABLE_WEBRTC)
extern const char kChromeUIWebRtcLogsURL[];
#endif

extern const char kChromeUIAboutHost[];
extern const char kChromeUIAboutPageFrameHost[];
extern const char kChromeUIBlankHost[];
extern const char kChromeUIAppLauncherPageHost[];
extern const char kChromeUIAppListStartPageHost[];
extern const char kChromeUIBookmarksHost[];
extern const char kChromeUICacheHost[];
extern const char kChromeUICertificateViewerHost[];
extern const char kChromeUIChromeSigninHost[];
extern const char kChromeUIChromeURLsHost[];
extern const char kChromeUICloudPrintResourcesHost[];
extern const char kChromeUICloudPrintSetupHost[];
extern const char kChromeUIConflictsHost[];
extern const char kChromeUIConstrainedHTMLTestHost[];
extern const char kChromeUICrashesHost[];
extern const char kChromeUICrashHost[];
extern const char kChromeUICreditsHost[];
extern const char kChromeUIDefaultHost[];
extern const char kChromeUIDevicesHost[];
extern const char kChromeUIDevToolsHost[];
extern const char kChromeUIDevToolsBundledPath[];
extern const char kChromeUIDevToolsRemotePath[];
extern const char kChromeUIDNSHost[];
extern const char kChromeUIDownloadsHost[];
extern const char kChromeUIDriveInternalsHost[];
extern const char kChromeUIEditSearchEngineDialogHost[];
extern const char kChromeUIEnhancedBookmarksHost[];
extern const char kChromeUIExtensionIconHost[];
extern const char kChromeUIExtensionInfoHost[];
extern const char kChromeUIExtensionsFrameHost[];
extern const char kChromeUIExtensionsHost[];
extern const char kChromeUIFaviconHost[];
extern const char kChromeUIFeedbackHost[];
extern const char kChromeUIFlagsHost[];
extern const char kChromeUIFlashHost[];
extern const char kChromeUIHelpFrameHost[];
extern const char kChromeUIHelpHost[];
extern const char kChromeUIHangHost[];
extern const char kChromeUIHistoryHost[];
extern const char kChromeUIHistoryFrameHost[];
extern const char kChromeUIIdentityInternalsHost[];
extern const char kChromeUIInspectHost[];
extern const char kChromeUIInstantHost[];
extern const char kChromeUIIPCHost[];
extern const char kChromeUIKillHost[];
extern const char kChromeUIManagedUserPassphrasePageHost[];
extern const char kChromeUIMemoryHost[];
extern const char kChromeUIMemoryInternalsHost[];
extern const char kChromeUIMemoryRedirectHost[];
extern const char kChromeUIMetroFlowHost[];
extern const char kChromeUINaClHost[];
extern const char kChromeUINetExportHost[];
extern const char kChromeUINetInternalsHost[];
extern const char kChromeUINewTabHost[];
extern const char kChromeUIOmniboxHost[];
extern const char kChromeUIPerformanceMonitorHost[];
extern const char kChromeUIPluginsHost[];
extern const char kChromeUIComponentsHost[];
extern const char kChromeUIPolicyHost[];
extern const char kChromeUIProfileSigninConfirmationHost[];
extern const char kChromeUIUserManagerHost[];
extern const char kChromeUIPredictorsHost[];
extern const char kChromeUIPrintHost[];
extern const char kChromeUIProfilerHost[];
extern const char kChromeUIQuotaInternalsHost[];
extern const char kChromeUIQuitHost[];
extern const char kChromeUIRestartHost[];
extern const char kChromeUISessionFaviconHost[];
extern const char kChromeUISettingsHost[];
extern const char kChromeUISettingsFrameHost[];
extern const char kChromeUIShorthangHost[];
extern const char kChromeUISignInInternalsHost[];
extern const char kChromeUISuggestionsInternalsHost[];
extern const char kChromeUISSLClientCertificateSelectorHost[];
extern const char kChromeUIStatsHost[];
extern const char kChromeUISyncHost[];
extern const char kChromeUISyncFileSystemInternalsHost[];
extern const char kChromeUISyncInternalsHost[];
extern const char kChromeUISyncResourcesHost[];
extern const char kChromeUITaskManagerHost[];
extern const char kChromeUITermsHost[];
extern const char kChromeUIThemeHost[];
extern const char kChromeUIThumbnailHost[];
extern const char kChromeUIThumbnailHost2[];
extern const char kChromeUIThumbnailListHost[];
extern const char kChromeUITouchIconHost[];
extern const char kChromeUITranslateInternalsHost[];
extern const char kChromeUIUberFrameHost[];
extern const char kChromeUIUberHost[];
extern const char kChromeUIUserActionsHost[];
extern const char kChromeUIVersionHost[];
extern const char kChromeUIWorkersHost[];

extern const char kChromeUIScreenshotPath[];
extern const char kChromeUIThemePath[];

#if defined(OS_ANDROID)
extern const char kChromeUIWelcomeHost[];
#endif

#if defined(OS_POSIX) && !defined(OS_MACOSX) && !defined(OS_ANDROID)
extern const char kChromeUILinuxProxyConfigHost[];
extern const char kChromeUISandboxHost[];
#endif

#if defined(OS_CHROMEOS)
extern const char kChromeUIActivationMessageHost[];
extern const char kChromeUIAppLaunchHost[];
extern const char kChromeUIBluetoothPairingHost[];
extern const char kChromeUIChargerReplacementHost[];
extern const char kChromeUIChooseMobileNetworkHost[];
extern const char kChromeUICryptohomeHost[];
extern const char kChromeUIDiagnosticsHost[];
extern const char kChromeUIDiscardsHost[];
extern const char kChromeUIFirstRunHost[];
extern const char kChromeUIIdleLogoutDialogHost[];
extern const char kChromeUIImageBurnerHost[];
extern const char kChromeUIKeyboardOverlayHost[];
extern const char kChromeUILockScreenHost[];
extern const char kChromeUILoginContainerHost[];
extern const char kChromeUILoginHost[];
extern const char kChromeUIMediaplayerHost[];
extern const char kChromeUIMobileSetupHost[];
extern const char kChromeUINetworkHost[];
extern const char kChromeUIOobeHost[];
extern const char kChromeUIOSCreditsHost[];
extern const char kChromeUIProxySettingsHost[];
extern const char kChromeUIRotateHost[];
extern const char kChromeUIScreenlockIconHost[];
extern const char kChromeUISimUnlockHost[];
extern const char kChromeUISlideshowHost[];
extern const char kChromeUISlowHost[];
extern const char kChromeUISlowTraceHost[];
extern const char kChromeUISystemInfoHost[];
extern const char kChromeUIUserImageHost[];

extern const char kChromeUIMenu[];
extern const char kChromeUINetworkMenu[];
extern const char kChromeUIWrenchMenu[];

extern const char kEULAPathFormat[];
extern const char kOemEulaURLPath[];
extern const char kOnlineEulaURLPath[];
#endif

#if (defined(OS_LINUX) && defined(TOOLKIT_VIEWS)) || defined(USE_AURA)
extern const char kChromeUITabModalConfirmDialogHost[];
#endif

#if defined(ENABLE_WEBRTC)
extern const char kChromeUIWebRtcLogsHost[];
#endif

extern const char kAutofillSubPage[];
extern const char kClearBrowserDataSubPage[];
extern const char kContentSettingsExceptionsSubPage[];
extern const char kContentSettingsSubPage[];
extern const char kCreateProfileSubPage[];
extern const char kExtensionsSubPage[];
extern const char kHandlerSettingsSubPage[];
extern const char kImportDataSubPage[];
extern const char kLanguageOptionsSubPage[];
extern const char kManagedUserSettingsSubPage[];
extern const char kManageProfileSubPage[];
extern const char kPasswordManagerSubPage[];
extern const char kResetProfileSettingsSubPage[];
extern const char kSearchEnginesSubPage[];
extern const char kSearchSubPage[];
extern const char kSearchUsersSubPage[];
extern const char kSyncSetupSubPage[];
#if defined(OS_CHROMEOS)
extern const char kInternetOptionsSubPage[];
extern const char kBluetoothAddDeviceSubPage[];
extern const char kChangeProfilePictureSubPage[];
#endif

extern const char kExtensionConfigureCommandsSubPage[];

extern const char kExtensionInvalidRequestURL[];
extern const char kExtensionResourceInvalidRequestURL[];

extern const char kSyncGoogleDashboardURL[];

extern const char kAutoPasswordGenerationLearnMoreURL[];

extern const char kPasswordManagerLearnMoreURL[];

extern const char kChromeHelpViaKeyboardURL[];
extern const char kChromeHelpViaMenuURL[];
extern const char kChromeHelpViaWebUIURL[];

#if defined(OS_CHROMEOS)
extern const char kChromeAccessibilityHelpURL[];
extern const char kChromeAccessibilitySettingsURL[];
#endif

#if defined (ENABLE_ONE_CLICK_SIGNIN)
extern const char kChromeSyncLearnMoreURL[];

extern const char kChromeSyncMergeTroubleshootingURL[];
#endif

extern const char kChromeEnterpriseSignInLearnMoreURL[];

extern const char kResetProfileSettingsLearnMoreURL[];

extern const char kSupervisedUserManagementURL[];

extern const char kSupervisedUserManagementDisplayURL[];

extern const char kSettingsSearchHelpURL[];

extern const char kAboutGoogleTranslateURL[];

extern const char kOmniboxLearnMoreURL[];

extern const char kPageInfoHelpCenterURL[];

extern const char kCrashReasonURL[];

extern const char kKillReasonURL[];

extern const char kPrivacyLearnMoreURL[];

extern const char kDoNotTrackLearnMoreURL[];

#if defined(OS_CHROMEOS)

extern const char kAttestationForContentProtectionLearnMoreURL[];

extern const char kEnhancedPlaybackNotificationLearnMoreURL[];
#endif

extern const char kChromiumProjectURL[];

extern const char kLearnMoreReportingURL[];

extern const char kOutdatedPluginLearnMoreURL[];

extern const char kBlockedPluginLearnMoreURL[];

extern const char kSpeechInputAboutURL[];

extern const char kLearnMoreRegisterProtocolHandlerURL[];

extern const char kSyncLearnMoreURL[];

extern const char kDownloadScanningLearnMoreURL[];

extern const char kDownloadInterruptedLearnMoreURL[];

extern const char kSyncEverythingLearnMoreURL[];

extern const char kAppLauncherHelpURL[];

extern const char kSyncEncryptionHelpURL[];

extern const char kSyncErrorsHelpURL[];

#if defined(OS_CHROMEOS)
extern const char kNaturalScrollHelpURL[];

extern const char kLearnMoreEnterpriseURL[];
#endif

extern const char kRemoveNonCWSExtensionURL[];

extern const char kNotificationsHelpURL[];

extern const char kNotificationWelcomeLearnMoreURL[];

extern const char* const kChromeHostURLs[];
extern const size_t kNumberOfChromeHostURLs;

extern const char* const kChromeDebugURLs[];
extern const int kNumberOfChromeDebugURLs;

extern const char kExtensionResourceScheme[];

extern const char kChromeNativeScheme[];

extern const char kChromeSearchScheme[];

extern const char kChromeSearchLocalNtpHost[];
extern const char kChromeSearchLocalNtpUrl[];
extern const char kChromeSearchOnlineNtpHost[];

extern const char kChromeSearchMostVisitedHost[];
extern const char kChromeSearchMostVisitedUrl[];

#if defined(OS_CHROMEOS)
extern const char kCrosScheme[];
extern const char kDriveScheme[];
#endif

extern const char kCloudPrintLearnMoreURL[];

extern const char kCloudPrintNoDestinationsLearnMoreURL[];

extern const char kSafeSearchSafeParameter[];
extern const char kSafeSearchSsuiParameter[];

extern const char kMediaAccessLearnMoreUrl[];

}  

#endif  
