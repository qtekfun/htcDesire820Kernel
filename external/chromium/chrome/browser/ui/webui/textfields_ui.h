// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_TEXTFIELDS_UI_H_
#define CHROME_BROWSER_UI_WEBUI_TEXTFIELDS_UI_H_
#pragma once

#include <string>

#include "chrome/browser/ui/webui/chrome_url_data_manager.h"
#include "content/browser/webui/web_ui.h"

class RefCountedMemory;

class TextfieldsUIHTMLSource : public ChromeURLDataManager::DataSource {
 public:
  TextfieldsUIHTMLSource();

  
  
  virtual void StartDataRequest(const std::string& path,
                                bool is_incognito,
                                int request_id);
  virtual std::string GetMimeType(const std::string& path) const;

 private:
  virtual ~TextfieldsUIHTMLSource();

  DISALLOW_COPY_AND_ASSIGN(TextfieldsUIHTMLSource);
};

class TextfieldsDOMHandler : public WebUIMessageHandler {
 public:
  TextfieldsDOMHandler();

  
  
  
  virtual void HandleTextfieldValue(const ListValue* args);

 protected:
  virtual void RegisterMessages();

 private:
  DISALLOW_COPY_AND_ASSIGN(TextfieldsDOMHandler);
};

class TextfieldsUI : public WebUI {
 public:
  explicit TextfieldsUI(TabContents* contents);

  const std::wstring& text() const { return text_; }
  void set_text(const std::wstring& text) { text_ = text; }

 private:
  std::wstring text_;

  DISALLOW_COPY_AND_ASSIGN(TextfieldsUI);
};

#endif  
