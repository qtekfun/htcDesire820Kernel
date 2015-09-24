// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OMNIBOX_OMNIBOX_UI_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OMNIBOX_OMNIBOX_UI_HANDLER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "chrome/browser/autocomplete/autocomplete_controller_delegate.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "content/public/browser/web_ui_message_handler.h"

class AutocompleteController;
class Profile;

namespace base {
class ListValue;
}

class OmniboxUIHandler : public AutocompleteControllerDelegate,
                         public content::WebUIMessageHandler {
 public:
  explicit OmniboxUIHandler(Profile* profile);
  virtual ~OmniboxUIHandler();

  
  
  
  
  
  
  virtual void OnResultChanged(bool default_match_changed) OVERRIDE;

 protected:
  
  
  
  virtual void RegisterMessages() OVERRIDE;

 private:
  
  
  
  
  
  
  
  
  void StartOmniboxQuery(const base::ListValue* input);

  
  
  
  void AddResultToDictionary(const std::string& prefix,
                             ACMatches::const_iterator result_it,
                             ACMatches::const_iterator end,
                             base::DictionaryValue* output);

  
  
  
  bool LookupIsTypedHost(const base::string16& host, bool* is_typed_host) const;

  
  
  void ResetController();

  
  
  scoped_ptr<AutocompleteController> controller_;

  
  
  
  base::Time time_omnibox_started_;

  
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(OmniboxUIHandler);
};

#endif  
