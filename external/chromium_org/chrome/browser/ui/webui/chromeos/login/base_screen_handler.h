// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_BASE_SCREEN_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_BASE_SCREEN_HANDLER_H_

#include <string>

#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/callback.h"
#include "base/strings/string16.h"
#include "chrome/browser/ui/webui/chromeos/login/base_screen_handler_utils.h"
#include "content/public/browser/web_ui.h"
#include "content/public/browser/web_ui_message_handler.h"
#include "ui/gfx/native_widget_types.h"

namespace base {
class DictionaryValue;
class ListValue;
class Value;
}

namespace chromeos {

class LocalizedValuesBuilder {
 public:
  explicit LocalizedValuesBuilder(base::DictionaryValue* dict);
  
  
  void Add(const std::string& key, const std::string& message);

  
  
  void Add(const std::string& key, const base::string16& message);

  
  
  void Add(const std::string& key, int message_id);

  
  
  
  void AddF(const std::string& key,
            int message_id,
            const base::string16& a);

  
  
  
  void AddF(const std::string& key,
            int message_id,
            const base::string16& a,
            const base::string16& b);

  
  
  
  void AddF(const std::string& key,
            int message_id,
            int message_id_a);

  
  
  
  
  void AddF(const std::string& key,
            int message_id,
            int message_id_a,
            int message_id_b);
 private:
  
  base::DictionaryValue* dict_;
};

class BaseScreenHandler : public content::WebUIMessageHandler {
 public:
  
  BaseScreenHandler();

  
  explicit BaseScreenHandler(const std::string& js_screen_path);

  virtual ~BaseScreenHandler();

  
  void GetLocalizedStrings(
      base::DictionaryValue* localized_strings);

  
  
  void InitializeBase();

  void set_async_assets_load_id(const std::string& async_assets_load_id) {
    async_assets_load_id_ = async_assets_load_id;
  }
  const std::string& async_assets_load_id() const {
    return async_assets_load_id_;
  }

 protected:
  
  virtual void DeclareLocalizedValues(LocalizedValuesBuilder* builder) = 0;

  
  
  
  virtual void GetAdditionalParameters(base::DictionaryValue* parameters);

  
  void CallJS(const std::string& method);

  template<typename A1>
  void CallJS(const std::string& method, const A1& arg1) {
    web_ui()->CallJavascriptFunction(FullMethodPath(method), MakeValue(arg1));
  }

  template<typename A1, typename A2>
  void CallJS(const std::string& method, const A1& arg1, const A2& arg2) {
    web_ui()->CallJavascriptFunction(FullMethodPath(method), MakeValue(arg1),
                                     MakeValue(arg2));
  }

  template<typename A1, typename A2, typename A3>
  void CallJS(const std::string& method,
              const A1& arg1,
              const A2& arg2,
              const A3& arg3) {
    web_ui()->CallJavascriptFunction(FullMethodPath(method),
                                     MakeValue(arg1),
                                     MakeValue(arg2),
                                     MakeValue(arg3));
  }

  template<typename A1, typename A2, typename A3, typename A4>
  void CallJS(const std::string& method,
              const A1& arg1,
              const A2& arg2,
              const A3& arg3,
              const A4& arg4) {
    web_ui()->CallJavascriptFunction(FullMethodPath(method),
                                     MakeValue(arg1),
                                     MakeValue(arg2),
                                     MakeValue(arg3),
                                     MakeValue(arg4));
  }

  
  template<typename T>
  void AddRawCallback(const std::string& name,
                      void (T::*method)(const base::ListValue* args)) {
    web_ui()->RegisterMessageCallback(
        name,
        base::Bind(method, base::Unretained(static_cast<T*>(this))));
  }

  template<typename T>
  void AddCallback(const std::string& name, void (T::*method)()) {
    base::Callback<void()> callback =
        base::Bind(method, base::Unretained(static_cast<T*>(this)));
    web_ui()->RegisterMessageCallback(
        name, base::Bind(&CallbackWrapper0, callback));
  }

  template<typename T, typename A1>
  void AddCallback(const std::string& name, void (T::*method)(A1 arg1)) {
    base::Callback<void(A1)> callback =
        base::Bind(method, base::Unretained(static_cast<T*>(this)));
    web_ui()->RegisterMessageCallback(
        name, base::Bind(&CallbackWrapper1<A1>, callback));
  }

  template<typename T, typename A1, typename A2>
  void AddCallback(const std::string& name,
                   void (T::*method)(A1 arg1, A2 arg2)) {
    base::Callback<void(A1, A2)> callback =
        base::Bind(method, base::Unretained(static_cast<T*>(this)));
    web_ui()->RegisterMessageCallback(
        name, base::Bind(&CallbackWrapper2<A1, A2>, callback));
  }

  template<typename T, typename A1, typename A2, typename A3>
  void AddCallback(const std::string& name,
                   void (T::*method)(A1 arg1, A2 arg2, A3 arg3)) {
    base::Callback<void(A1, A2, A3)> callback =
        base::Bind(method, base::Unretained(static_cast<T*>(this)));
    web_ui()->RegisterMessageCallback(
        name, base::Bind(&CallbackWrapper3<A1, A2, A3>, callback));
  }

  template<typename T, typename A1, typename A2, typename A3, typename A4>
  void AddCallback(const std::string& name,
                   void (T::*method)(A1 arg1, A2 arg2, A3 arg3, A4 arg4)) {
    base::Callback<void(A1, A2, A3, A4)> callback =
        base::Bind(method, base::Unretained(static_cast<T*>(this)));
    web_ui()->RegisterMessageCallback(
        name, base::Bind(&CallbackWrapper4<A1, A2, A3, A4>, callback));
  }

  
  virtual void Initialize() = 0;

  
  
  void ShowScreen(const char* screen, const base::DictionaryValue* data);

  
  bool page_is_ready() const { return page_is_ready_; }

  
  virtual gfx::NativeWindow GetNativeWindow();

 private:
  
  
  std::string FullMethodPath(const std::string& method) const;

  
  bool page_is_ready_;

  base::DictionaryValue* localized_values_;

  
  
  
  std::string js_screen_path_prefix_;

  
  
  
  std::string async_assets_load_id_;

  DISALLOW_COPY_AND_ASSIGN(BaseScreenHandler);
};

}  

#endif  
