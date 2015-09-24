// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_EXTENSIONS_API_FONT_SETTINGS_FONT_SETTINGS_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_FONT_SETTINGS_FONT_SETTINGS_API_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/prefs/pref_service.h"
#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "chrome/browser/extensions/chrome_extension_function.h"
#include "extensions/browser/event_router.h"

class Profile;

namespace extensions {

class FontSettingsEventRouter {
 public:
  
  
  
  explicit FontSettingsEventRouter(Profile* profile);
  virtual ~FontSettingsEventRouter();

 private:
  
  
  
  void AddPrefToObserve(const char* pref_name,
                        const char* event_name,
                        const char* key);

  
  
  void OnFontFamilyMapPrefChanged(const std::string& pref_name);

  
  
  
  void OnFontNamePrefChanged(const std::string& pref_name,
                             const std::string& generic_family,
                             const std::string& script);

  
  
  
  
  void OnFontPrefChanged(const std::string& event_name,
                         const std::string& key,
                         const std::string& pref_name);

  
  PrefChangeRegistrar registrar_;

  
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(FontSettingsEventRouter);
};

class FontSettingsAPI : public ProfileKeyedAPI {
 public:
  explicit FontSettingsAPI(Profile* profile);
  virtual ~FontSettingsAPI();

  
  static ProfileKeyedAPIFactory<FontSettingsAPI>* GetFactoryInstance();

 private:
  friend class ProfileKeyedAPIFactory<FontSettingsAPI>;

  
  static const char* service_name() {
    return "FontSettingsAPI";
  }
  static const bool kServiceIsNULLWhileTesting = true;

  scoped_ptr<FontSettingsEventRouter> font_settings_event_router_;
};

class FontSettingsClearFontFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fontSettings.clearFont", FONTSETTINGS_CLEARFONT)

 protected:
  
  
  virtual ~FontSettingsClearFontFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class FontSettingsGetFontFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fontSettings.getFont", FONTSETTINGS_GETFONT)

 protected:
  virtual ~FontSettingsGetFontFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class FontSettingsSetFontFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fontSettings.setFont", FONTSETTINGS_SETFONT)

 protected:
  virtual ~FontSettingsSetFontFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class FontSettingsGetFontListFunction : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fontSettings.getFontList",
                             FONTSETTINGS_GETFONTLIST)

 protected:
  virtual ~FontSettingsGetFontListFunction() {}

  
  virtual bool RunImpl() OVERRIDE;

 private:
  void FontListHasLoaded(scoped_ptr<base::ListValue> list);
  bool CopyFontsToResult(base::ListValue* fonts);
};

class ClearFontPrefExtensionFunction : public ChromeSyncExtensionFunction {
 protected:
  virtual ~ClearFontPrefExtensionFunction() {}

  
  virtual bool RunImpl() OVERRIDE;

  
  
  virtual const char* GetPrefName() = 0;
};

class GetFontPrefExtensionFunction : public ChromeSyncExtensionFunction {
 protected:
  virtual ~GetFontPrefExtensionFunction() {}

  
  virtual bool RunImpl() OVERRIDE;

  
  
  virtual const char* GetPrefName() = 0;

  
  
  virtual const char* GetKey() = 0;
};

class SetFontPrefExtensionFunction : public ChromeSyncExtensionFunction {
 protected:
  virtual ~SetFontPrefExtensionFunction() {}

  
  virtual bool RunImpl() OVERRIDE;

  
  
  virtual const char* GetPrefName() = 0;

  
  
  virtual const char* GetKey() = 0;
};


class FontSettingsClearDefaultFontSizeFunction
    : public ClearFontPrefExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fontSettings.clearDefaultFontSize",
                             FONTSETTINGS_CLEARDEFAULTFONTSIZE)

 protected:
  virtual ~FontSettingsClearDefaultFontSizeFunction() {}

  
  virtual const char* GetPrefName() OVERRIDE;
};

class FontSettingsGetDefaultFontSizeFunction
    : public GetFontPrefExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fontSettings.getDefaultFontSize",
                             FONTSETTINGS_GETDEFAULTFONTSIZE)

 protected:
  virtual ~FontSettingsGetDefaultFontSizeFunction() {}

  
  virtual const char* GetPrefName() OVERRIDE;
  virtual const char* GetKey() OVERRIDE;
};

class FontSettingsSetDefaultFontSizeFunction
    : public SetFontPrefExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fontSettings.setDefaultFontSize",
                             FONTSETTINGS_SETDEFAULTFONTSIZE)

 protected:
  virtual ~FontSettingsSetDefaultFontSizeFunction() {}

  
  virtual const char* GetPrefName() OVERRIDE;
  virtual const char* GetKey() OVERRIDE;
};

class FontSettingsClearDefaultFixedFontSizeFunction
    : public ClearFontPrefExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fontSettings.clearDefaultFixedFontSize",
                             FONTSETTINGS_CLEARDEFAULTFIXEDFONTSIZE)

 protected:
  virtual ~FontSettingsClearDefaultFixedFontSizeFunction() {}

  
  virtual const char* GetPrefName() OVERRIDE;
};

class FontSettingsGetDefaultFixedFontSizeFunction
    : public GetFontPrefExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fontSettings.getDefaultFixedFontSize",
                             FONTSETTINGS_GETDEFAULTFIXEDFONTSIZE)

 protected:
  virtual ~FontSettingsGetDefaultFixedFontSizeFunction() {}

  
  virtual const char* GetPrefName() OVERRIDE;
  virtual const char* GetKey() OVERRIDE;
};

class FontSettingsSetDefaultFixedFontSizeFunction
    : public SetFontPrefExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fontSettings.setDefaultFixedFontSize",
                             FONTSETTINGS_SETDEFAULTFIXEDFONTSIZE)

 protected:
  virtual ~FontSettingsSetDefaultFixedFontSizeFunction() {}

  
  virtual const char* GetPrefName() OVERRIDE;
  virtual const char* GetKey() OVERRIDE;
};

class FontSettingsClearMinimumFontSizeFunction
    : public ClearFontPrefExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fontSettings.clearMinimumFontSize",
                             FONTSETTINGS_CLEARMINIMUMFONTSIZE)

 protected:
  virtual ~FontSettingsClearMinimumFontSizeFunction() {}

  
  virtual const char* GetPrefName() OVERRIDE;
};

class FontSettingsGetMinimumFontSizeFunction
    : public GetFontPrefExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fontSettings.getMinimumFontSize",
                             FONTSETTINGS_GETMINIMUMFONTSIZE)

 protected:
  virtual ~FontSettingsGetMinimumFontSizeFunction() {}

  
  virtual const char* GetPrefName() OVERRIDE;
  virtual const char* GetKey() OVERRIDE;
};

class FontSettingsSetMinimumFontSizeFunction
    : public SetFontPrefExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fontSettings.setMinimumFontSize",
                             FONTSETTINGS_SETMINIMUMFONTSIZE)

 protected:
  virtual ~FontSettingsSetMinimumFontSizeFunction() {}

  
  virtual const char* GetPrefName() OVERRIDE;
  virtual const char* GetKey() OVERRIDE;
};

}  

#endif  
