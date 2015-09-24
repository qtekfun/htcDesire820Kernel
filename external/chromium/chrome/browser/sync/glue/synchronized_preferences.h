// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_SYNC_GLUE_SYNCHRONIZED_PREFERENCES_H_
#define CHROME_BROWSER_SYNC_GLUE_SYNCHRONIZED_PREFERENCES_H_
#pragma once

#include "chrome/browser/translate/translate_prefs.h"
#include "chrome/common/pref_names.h"

namespace browser_sync {

static const char* kSynchronizedPreferences[] = {
  
  prefs::kRestoreOnStartup,
  prefs::kURLsToRestoreOnStartup,
  prefs::kShowBookmarkBar,
  prefs::kHomePageIsNewTabPage,
  prefs::kHomePage,
  prefs::kShowHomeButton,
  
  

  
  prefs::kPasswordManagerEnabled,
  prefs::kAutofillEnabled,
  prefs::kUseCustomChromeFrame,

  
  
  prefs::kBlockThirdPartyCookies,
  prefs::kClearSiteDataOnExit,

  
  
  prefs::kDefaultContentSettings,
  prefs::kContentSettingsPatterns,

  
  
  prefs::kGeolocationContentSettings,
  prefs::kGeolocationDefaultContentSetting,

  
  prefs::kDesktopNotificationDefaultContentSetting,

  
  
  prefs::kDeleteBrowsingHistory,
  prefs::kDeleteDownloadHistory,
  prefs::kDeleteCache,
  prefs::kDeleteCookies,
  prefs::kDeletePasswords,
  prefs::kDeleteFormData,
  prefs::kDeleteTimePeriod,

  
  

  
  
  prefs::kDefaultCharset,
  
  
#if !defined(OS_MACOSX)
  prefs::kAcceptLanguages,
#endif
  prefs::kEnableSpellCheck,
  
  prefs::kApplicationLocale,

  
  prefs::kAlternateErrorPagesEnabled,
  prefs::kSearchSuggestEnabled,
  prefs::kNetworkPredictionEnabled,
  prefs::kSafeBrowsingEnabled,
  prefs::kEnableTranslate,
  
  
  prefs::kPromptForDownload,

  
  prefs::kExtensionsUIDeveloperMode,  

  
  
  

  
  prefs::kWebKitUsesUniversalDetector,

  
#if defined(OS_MACOSX)
  prefs::kAutofillAuxiliaryProfilesEnabled,
#endif

  
  TranslatePrefs::kPrefTranslateLanguageBlacklist,
  TranslatePrefs::kPrefTranslateSiteBlacklist,
  TranslatePrefs::kPrefTranslateWhitelists,
  TranslatePrefs::kPrefTranslateDeniedCount,
  TranslatePrefs::kPrefTranslateAcceptedCount,

  
  prefs::kDesktopNotificationAllowedOrigins,
  prefs::kDesktopNotificationDeniedOrigins,

  
  prefs::kConfirmToQuitEnabled,

#if defined(OS_CHROMEOS)
  
  prefs::kLanguageChewingAddPhraseDirection,
  prefs::kLanguageChewingAutoShiftCur,
  prefs::kLanguageChewingCandPerPage,
  prefs::kLanguageChewingEasySymbolInput,
  prefs::kLanguageChewingEscCleanAllBuf,
  prefs::kLanguageChewingForceLowercaseEnglish,
  prefs::kLanguageChewingHsuSelKeyType,
  prefs::kLanguageChewingKeyboardType,
  prefs::kLanguageChewingMaxChiSymbolLen,
  prefs::kLanguageChewingPhraseChoiceRearward,
  prefs::kLanguageChewingPlainZhuyin,
  prefs::kLanguageChewingSelKeys,
  prefs::kLanguageChewingSpaceAsSelection,
  prefs::kLanguageHangulKeyboard,
  prefs::kLanguageMozcHistoryLearningLevel,
  prefs::kLanguageMozcIncognitoMode,
  prefs::kLanguageMozcNumpadCharacterForm,
  prefs::kLanguageMozcPreeditMethod,
  prefs::kLanguageMozcPunctuationMethod,
  prefs::kLanguageMozcSessionKeymap,
  prefs::kLanguageMozcShiftKeyModeSwitch,
  prefs::kLanguageMozcSpaceCharacterForm,
  prefs::kLanguageMozcSuggestionsSize,
  prefs::kLanguageMozcSymbolMethod,
  prefs::kLanguageMozcUseAutoImeTurnOff,
  prefs::kLanguageMozcUseDateConversion,
  prefs::kLanguageMozcUseDictionarySuggest,
  prefs::kLanguageMozcUseHistorySuggest,
  prefs::kLanguageMozcUseNumberConversion,
  prefs::kLanguageMozcUseSingleKanjiConversion,
  prefs::kLanguageMozcUseSymbolConversion,
  prefs::kLanguagePinyinAutoCommit,
  prefs::kLanguagePinyinCommaPeriodPage,
  prefs::kLanguagePinyinCorrectPinyin,
  prefs::kLanguagePinyinDoublePinyin,
  prefs::kLanguagePinyinDoublePinyinSchema,
  prefs::kLanguagePinyinFuzzyPinyin,
  prefs::kLanguagePinyinInitChinese,
  prefs::kLanguagePinyinInitFull,
  prefs::kLanguagePinyinInitFullPunct,
  prefs::kLanguagePinyinInitSimplifiedChinese,
  prefs::kLanguagePinyinMinusEqualPage,
  prefs::kLanguagePinyinShiftSelectCandidate,
  prefs::kLanguagePinyinTradCandidate,
  prefs::kLanguagePreferredLanguages,
  prefs::kLanguagePreloadEngines,

  
  
  
  
  
  
  
  
  

  
  prefs::kLanguageXkbRemapAltKeyTo,
  prefs::kLanguageXkbRemapControlKeyTo,
  prefs::kLanguageXkbRemapSearchKeyTo,

  
  
  
  
  

  
  
  prefs::kShowPlanNotifications,

  
  
  prefs::kEnableScreenLock,

  
  
  prefs::kTapToClickEnabled,

  
  
  prefs::kUse24HourClock,
#endif
};

}  

#endif  
