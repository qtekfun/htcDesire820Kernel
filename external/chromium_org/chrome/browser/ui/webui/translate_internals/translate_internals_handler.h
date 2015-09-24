// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_TRANSLATE_INTERNALS_TRANSLATE_INTERNALS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_TRANSLATE_INTERNALS_TRANSLATE_INTERNALS_HANDLER_H_

#include <string>

#include "chrome/browser/translate/translate_manager.h"
#include "content/public/browser/web_ui_message_handler.h"
#include "content/public/common/webplugininfo.h"

struct LanguageDetectionDetails;
struct TranslateErrorDetails;
struct TranslateEventDetails;

namespace base {
class DictionaryValue;
class ListValue;
class Value;
}

class TranslateInternalsHandler : public content::WebUIMessageHandler,
                                  public TranslateManager::Observer {
 public:
  TranslateInternalsHandler();
  virtual ~TranslateInternalsHandler();

  
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void OnLanguageDetection(
      const LanguageDetectionDetails& details) OVERRIDE;
  virtual void OnTranslateError(
      const TranslateErrorDetails& details) OVERRIDE;
  virtual void OnTranslateEvent(
      const TranslateEventDetails& details) OVERRIDE;

 private:
  
  
  void OnRemovePrefItem(const base::ListValue* args);

  
  
  
  
  void OnRequestInfo(const base::ListValue* args);

  
  void SendMessageToJs(const std::string& message, const base::Value& value);

  
  void SendPrefsToJs();

  
  void SendSupportedLanguagesToJs();

  DISALLOW_COPY_AND_ASSIGN(TranslateInternalsHandler);
};

#endif  
