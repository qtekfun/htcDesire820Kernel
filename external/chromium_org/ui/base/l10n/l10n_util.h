// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef UI_BASE_L10N_L10N_UTIL_H_
#define UI_BASE_L10N_L10N_UTIL_H_

#include <string>
#include <vector>

#include "base/strings/string16.h"
#include "base/strings/string_util.h"
#include "ui/base/ui_export.h"

#if defined(OS_MACOSX)
#include "ui/base/l10n/l10n_util_mac.h"
#endif  

namespace l10n_util {
UI_EXPORT bool CheckAndResolveLocale(const std::string& locale,
                                     std::string* resolved_locale);

UI_EXPORT std::string GetApplicationLocale(const std::string& pref_locale);

UI_EXPORT bool IsLocaleNameTranslated(const char* locale,
                                      const std::string& display_locale);

bool IsLocaleSupportedByOS(const std::string& locale);


UI_EXPORT base::string16 GetDisplayNameForLocale(
    const std::string& locale,
    const std::string& display_locale,
    bool is_for_ui);

UI_EXPORT base::string16 GetDisplayNameForCountry(
    const std::string& country_code,
    const std::string& display_locale);

UI_EXPORT std::string NormalizeLocale(const std::string& locale);

UI_EXPORT void GetParentLocales(const std::string& current_locale,
                                std::vector<std::string>* parent_locales);

UI_EXPORT bool IsValidLocaleSyntax(const std::string& locale);


UI_EXPORT std::string GetStringUTF8(int message_id);
UI_EXPORT base::string16 GetStringUTF16(int message_id);

UI_EXPORT base::string16 GetStringFUTF16(
    int message_id,
    const std::vector<base::string16>& replacements,
    std::vector<size_t>* offsets);

UI_EXPORT base::string16 GetStringFUTF16(int message_id,
                                         const base::string16& a);
UI_EXPORT base::string16 GetStringFUTF16(int message_id,
                                         const base::string16& a,
                                         const base::string16& b);
UI_EXPORT base::string16 GetStringFUTF16(int message_id,
                                         const base::string16& a,
                                         const base::string16& b,
                                         const base::string16& c);
UI_EXPORT base::string16 GetStringFUTF16(int message_id,
                                         const base::string16& a,
                                         const base::string16& b,
                                         const base::string16& c,
                                         const base::string16& d);
UI_EXPORT base::string16 GetStringFUTF16(int message_id,
                                         const base::string16& a,
                                         const base::string16& b,
                                         const base::string16& c,
                                         const base::string16& d,
                                         const base::string16& e);
UI_EXPORT std::string GetStringFUTF8(int message_id,
                                     const base::string16& a);
UI_EXPORT std::string GetStringFUTF8(int message_id,
                                     const base::string16& a,
                                     const base::string16& b);
UI_EXPORT std::string GetStringFUTF8(int message_id,
                                     const base::string16& a,
                                     const base::string16& b,
                                     const base::string16& c);
UI_EXPORT std::string GetStringFUTF8(int message_id,
                                     const base::string16& a,
                                     const base::string16& b,
                                     const base::string16& c,
                                     const base::string16& d);

UI_EXPORT base::string16 GetStringFUTF16(int message_id,
                                         const base::string16& a,
                                         size_t* offset);
UI_EXPORT base::string16 GetStringFUTF16(int message_id,
                                         const base::string16& a,
                                         const base::string16& b,
                                         std::vector<size_t>* offsets);

UI_EXPORT base::string16 GetStringFUTF16Int(int message_id, int a);
base::string16 GetStringFUTF16Int(int message_id, int64 a);

UI_EXPORT base::string16 GetPluralStringFUTF16(
    const std::vector<int>& message_ids,
    int number);
UI_EXPORT std::string GetPluralStringFUTF8(const std::vector<int>& message_ids,
                                           int number);

UI_EXPORT void SortStrings16(const std::string& locale,
                             std::vector<base::string16>* strings);

UI_EXPORT const std::vector<std::string>& GetAvailableLocales();

UI_EXPORT void GetAcceptLanguagesForLocale(
    const std::string& display_locale,
    std::vector<std::string>* locale_codes);

UI_EXPORT int GetLocalizedContentsWidthInPixels(int pixel_resource_id);

}  

#endif  
