// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_OMNIBOX_OMNIBOX_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_OMNIBOX_OMNIBOX_API_H_

#include <set>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "chrome/browser/extensions/chrome_extension_function.h"
#include "chrome/browser/extensions/extension_icon_manager.h"
#include "chrome/browser/search_engines/template_url_service.h"
#include "chrome/common/extensions/api/omnibox.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/window_open_disposition.h"

class Profile;
class TemplateURL;
class TemplateURLService;

namespace base {
class ListValue;
}

namespace content {
class WebContents;
}

namespace gfx {
class Image;
}

namespace extensions {

class ExtensionOmniboxEventRouter {
 public:
  
  
  static void OnInputStarted(
      Profile* profile, const std::string& extension_id);

  
  
  
  static bool OnInputChanged(
      Profile* profile,
      const std::string& extension_id,
      const std::string& input, int suggest_id);

  
  static void OnInputEntered(
      content::WebContents* web_contents,
      const std::string& extension_id,
      const std::string& input,
      WindowOpenDisposition disposition);

  
  
  static void OnInputCancelled(
      Profile* profile, const std::string& extension_id);

 private:
  DISALLOW_COPY_AND_ASSIGN(ExtensionOmniboxEventRouter);
};

class OmniboxSendSuggestionsFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("omnibox.sendSuggestions", OMNIBOX_SENDSUGGESTIONS)

 protected:
  virtual ~OmniboxSendSuggestionsFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class OmniboxAPI : public ProfileKeyedAPI,
                   public content::NotificationObserver {
 public:
  explicit OmniboxAPI(Profile* profile);
  virtual ~OmniboxAPI();

  
  static ProfileKeyedAPIFactory<OmniboxAPI>* GetFactoryInstance();

  
  static OmniboxAPI* Get(Profile* profile);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void Shutdown() OVERRIDE;

  
  gfx::Image GetOmniboxIcon(const std::string& extension_id);

  
  
  gfx::Image GetOmniboxPopupIcon(const std::string& extension_id);

 private:
  friend class ProfileKeyedAPIFactory<OmniboxAPI>;

  typedef std::set<const Extension*> PendingExtensions;

  void OnTemplateURLsLoaded();

  
  static const char* service_name() {
    return "OmniboxAPI";
  }
  static const bool kServiceRedirectedInIncognito = true;

  Profile* profile_;

  TemplateURLService* url_service_;

  
  
  PendingExtensions pending_extensions_;

  content::NotificationRegistrar registrar_;

  
  ExtensionIconManager omnibox_icon_manager_;
  ExtensionIconManager omnibox_popup_icon_manager_;

  scoped_ptr<TemplateURLService::Subscription> template_url_sub_;

  DISALLOW_COPY_AND_ASSIGN(OmniboxAPI);
};

template <>
void ProfileKeyedAPIFactory<OmniboxAPI>::DeclareFactoryDependencies();

class OmniboxSetDefaultSuggestionFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("omnibox.setDefaultSuggestion",
                             OMNIBOX_SETDEFAULTSUGGESTION)

 protected:
  virtual ~OmniboxSetDefaultSuggestionFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

void ApplyDefaultSuggestionForExtensionKeyword(
    Profile* profile,
    const TemplateURL* keyword,
    const base::string16& remaining_input,
    AutocompleteMatch* match);

ACMatchClassifications StyleTypesToACMatchClassifications(
    const api::omnibox::SuggestResult &suggestion);

}  

#endif  
