// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_PROFILE_RESETTER_AUTOMATIC_PROFILE_RESETTER_MEMENTOS_H_
#define CHROME_BROWSER_PROFILE_RESETTER_AUTOMATIC_PROFILE_RESETTER_MEMENTOS_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"

namespace base {
class FilePath;
}

class Profile;

class PreferenceHostedPromptMemento {
 public:
  explicit PreferenceHostedPromptMemento(Profile* profile);
  ~PreferenceHostedPromptMemento();

  std::string ReadValue() const;
  void StoreValue(const std::string& value);

 private:
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(PreferenceHostedPromptMemento);
};

class LocalStateHostedPromptMemento {
 public:
  explicit LocalStateHostedPromptMemento(Profile* profile);
  ~LocalStateHostedPromptMemento();

  std::string ReadValue() const;
  void StoreValue(const std::string& value);

 private:
  
  
  std::string GetProfileKey() const;

  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(LocalStateHostedPromptMemento);
};

class FileHostedPromptMemento {
 public:
  typedef base::Callback<void(const std::string&)> ReadValueCallback;

  explicit FileHostedPromptMemento(Profile* profile);
  ~FileHostedPromptMemento();

  
  
  
  void ReadValue(const ReadValueCallback& callback) const;

  
  
  void StoreValue(const std::string& value);

 private:
  static std::string ReadValueOnFileThread(
      const base::FilePath& memento_file_path);
  static void StoreValueOnFileThread(const base::FilePath& memento_file_path,
                                     const std::string& value);

  
  
  base::FilePath GetMementoFilePath() const;

  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(FileHostedPromptMemento);
};

#endif  
