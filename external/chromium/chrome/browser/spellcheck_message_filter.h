// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SPELLCHECK_MESSAGE_FILTER_H_
#define CHROME_BROWSER_SPELLCHECK_MESSAGE_FILTER_H_

#include "content/browser/browser_message_filter.h"

class SpellCheckMessageFilter : public BrowserMessageFilter {
 public:
  SpellCheckMessageFilter();
  ~SpellCheckMessageFilter();

  
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok);

 private:
  void OnPlatformCheckSpelling(const string16& word, int tag, bool* correct);
  void OnPlatformFillSuggestionList(const string16& word,
                                    std::vector<string16>* suggestions);
  void OnGetDocumentTag(int* tag);
  void OnDocumentWithTagClosed(int tag);
  void OnShowSpellingPanel(bool show);
  void OnUpdateSpellingPanelWithMisspelledWord(const string16& word);
  void OnPlatformRequestTextCheck(int route_id,
                                  int identifier,
                                  int document_tag,
                                  const string16& text);
};

#endif  
