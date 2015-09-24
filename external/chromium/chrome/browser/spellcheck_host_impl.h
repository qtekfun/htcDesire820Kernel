// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SPELLCHECK_HOST_IMPL_H_
#define CHROME_BROWSER_SPELLCHECK_HOST_IMPL_H_
#pragma once

#include <string>
#include <vector>

#include "base/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/spellcheck_host.h"
#include "chrome/browser/spellcheck_host_observer.h"
#include "chrome/common/net/url_fetcher.h"

class SpellCheckHostImpl : public SpellCheckHost,
                           public URLFetcher::Delegate {
 public:
  SpellCheckHostImpl(SpellCheckHostObserver* observer,
                     const std::string& language,
                     net::URLRequestContextGetter* request_context_getter);

  void Initialize();

  

  virtual void UnsetObserver();

  virtual void AddWord(const std::string& word);

  virtual const base::PlatformFile& GetDictionaryFile() const;

  virtual const std::vector<std::string>& GetCustomWords() const;

  virtual const std::string& GetLastAddedFile() const;

  virtual const std::string& GetLanguage() const;

  virtual bool IsUsingPlatformChecker() const;

 private:
  
  friend class BrowserThread;
  friend class DeleteTask<SpellCheckHostImpl>;

  virtual ~SpellCheckHostImpl();

  
  
  
  
  
  
  void InitializeDictionaryLocation();

  
  
  void InitializeInternal();

  void InitializeOnFileThread();

  
  void InformObserverOfInitialization();

  
  void DownloadDictionary();

  
  void WriteWordToCustomDictionary(const std::string& word);

  
  
  virtual void OnURLFetchComplete(const URLFetcher* source,
                                  const GURL& url,
                                  const net::URLRequestStatus& status,
                                  int response_code,
                                  const ResponseCookies& cookies,
                                  const std::string& data);

  
  void SaveDictionaryData();

  
  SpellCheckHostObserver* observer_;

  
  FilePath bdict_file_path_;

  
  FilePath custom_dictionary_file_;

  
  std::string language_;

  
  base::PlatformFile file_;

  
  std::vector<std::string> custom_words_;

  
  
  bool tried_to_download_;

  
  bool use_platform_spellchecker_;

  
  std::string data_;

  
  
  net::URLRequestContextGetter* request_context_getter_;

  
  scoped_ptr<URLFetcher> fetcher_;
};

#endif  
