// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROME_URL_DATA_MANAGER_H_
#define CHROME_BROWSER_UI_WEBUI_CHROME_URL_DATA_MANAGER_H_
#pragma once

#include <string>

#include "base/memory/ref_counted.h"
#include "base/task.h"
#include "content/browser/browser_thread.h"

class ChromeURLDataManagerBackend;
class DictionaryValue;
class FilePath;
class MessageLoop;
class Profile;
class RefCountedMemory;

class ChromeURLDataManager {
 public:
  class DataSource;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  struct DeleteDataSource {
    static void Destruct(const DataSource* data_source) {
      ChromeURLDataManager::DeleteDataSource(data_source);
    }
  };

  
  
  
  
  
  
  
  
  
  class DataSource : public base::RefCountedThreadSafe<
      DataSource, DeleteDataSource> {
   public:
    
    DataSource(const std::string& source_name, MessageLoop* message_loop);

    
    
    
    virtual void StartDataRequest(const std::string& path,
                                  bool is_incognito,
                                  int request_id) = 0;

    
    
    virtual std::string GetMimeType(const std::string& path) const = 0;

    
    
    
    virtual void SendResponse(int request_id, RefCountedMemory* bytes);

    
    
    
    
    
    
    
    
    
    virtual MessageLoop* MessageLoopForRequestPath(const std::string& path)
        const;

    const std::string& source_name() const { return source_name_; }

    
    
    
    
    
    
    
    virtual bool ShouldReplaceExistingSource() const;

    static void SetFontAndTextDirection(DictionaryValue* localized_strings);

   protected:
    virtual ~DataSource();

   private:
    friend class ChromeURLDataManagerBackend;
    friend class ChromeURLDataManager;
    friend class DeleteTask<DataSource>;

    
    
    virtual void SendResponseOnIOThread(int request_id,
                                        scoped_refptr<RefCountedMemory> bytes);

    
    
    
    const std::string source_name_;

    
    
    MessageLoop* message_loop_;

    
    
    
    
    
    
    
    ChromeURLDataManagerBackend* backend_;
  };

  explicit ChromeURLDataManager(Profile* profile);
  ~ChromeURLDataManager();

  
  
  
  
  
  
  
  
  
  
  void AddDataSource(DataSource* source);

  
  
  static void DeleteDataSources();

 private:
  typedef std::vector<const ChromeURLDataManager::DataSource*> DataSources;

  
  
  
  static void DeleteDataSource(const DataSource* data_source);

  
  
  static bool IsScheduledForDeletion(const DataSource* data_source);

  Profile* profile_;

  
  static base::Lock delete_lock_;

  
  static DataSources* data_sources_;

  DISALLOW_COPY_AND_ASSIGN(ChromeURLDataManager);
};

#endif  
