// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SPELLCHECK_HOST_H_
#define CHROME_BROWSER_SPELLCHECK_HOST_H_
#pragma once

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/platform_file.h"
#include "content/browser/browser_thread.h"

class Profile;
class SpellCheckHostObserver;

namespace net {
class URLRequestContextGetter;
}

class SpellCheckHost
    : public base::RefCountedThreadSafe<SpellCheckHost,
                                        BrowserThread::DeleteOnFileThread> {
 public:
  virtual ~SpellCheckHost() {}

  
  static scoped_refptr<SpellCheckHost> Create(
      SpellCheckHostObserver* observer,
      const std::string& language,
      net::URLRequestContextGetter* request_context_getter);

  
  
  virtual void UnsetObserver() = 0;

  
  
  virtual void AddWord(const std::string& word) = 0;

  virtual const base::PlatformFile& GetDictionaryFile() const = 0;

  virtual const std::vector<std::string>& GetCustomWords() const = 0;

  virtual const std::string& GetLastAddedFile() const = 0;

  virtual const std::string& GetLanguage() const = 0;

  virtual bool IsUsingPlatformChecker() const = 0;

  
  
  
  
  
  static int GetSpellCheckLanguages(Profile* profile,
                                    std::vector<std::string>* languages);
};

#endif  
