// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UTILITY_PROCESS_HOST_H_
#define CHROME_BROWSER_UTILITY_PROCESS_HOST_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/task.h"
#include "chrome/common/extensions/update_manifest.h"
#include "content/browser/browser_child_process_host.h"
#include "content/browser/browser_thread.h"

class DictionaryValue;
class IndexedDBKey;
class ListValue;
class SerializedScriptValue;
class SkBitmap;

class UtilityProcessHost : public BrowserChildProcessHost {
 public:
  
  
  
  class Client : public base::RefCountedThreadSafe<Client> {
   public:
    Client() {}

    
    virtual void OnProcessCrashed(int exit_code) {}

    
    
    
    
    virtual void OnUnpackExtensionSucceeded(const DictionaryValue& manifest) {}

    
    
    virtual void OnUnpackExtensionFailed(const std::string& error_message) {}

    
    
    
    virtual void OnUnpackWebResourceSucceeded(
        const DictionaryValue& json_data) {}

    
    
    virtual void OnUnpackWebResourceFailed(const std::string& error_message) {}

    
    virtual void OnParseUpdateManifestSucceeded(
        const UpdateManifest::Results& results) {}

    
    
    virtual void OnParseUpdateManifestFailed(
        const std::string& error_message) {}

    
    
    virtual void OnDecodeImageSucceeded(
        const SkBitmap& decoded_image) {}

    
    virtual void OnDecodeImageFailed() {}

    
    
    
    
    virtual void OnIDBKeysFromValuesAndKeyPathSucceeded(
        int id, const std::vector<IndexedDBKey>& keys) {}

    
    
    
    virtual void OnIDBKeysFromValuesAndKeyPathFailed(int id) {}

    
    
    
    virtual void OnInjectIDBKeyFinished(
        const SerializedScriptValue& new_value) {}

    
    
    
    
    virtual void OnJSONParseSucceeded(const ListValue& wrapper) {}
    virtual void OnJSONParseFailed(const std::string& error_message) {}

   protected:
    friend class base::RefCountedThreadSafe<Client>;

    virtual ~Client() {}

   private:
    friend class UtilityProcessHost;

    bool OnMessageReceived(const IPC::Message& message);

    DISALLOW_COPY_AND_ASSIGN(Client);
  };

  UtilityProcessHost(Client* client, BrowserThread::ID client_thread_id);
  virtual ~UtilityProcessHost();

  
  
  
  
  bool StartExtensionUnpacker(const FilePath& extension);

  
  
  
  
  
  
  bool StartWebResourceUnpacker(const std::string& data);

  
  bool StartUpdateManifestParse(const std::string& xml);

  
  
  
  bool StartImageDecoding(const std::vector<unsigned char>& encoded_data);
  bool StartImageDecodingBase64(const std::string& base64_encoded_data);

  
  
  bool StartIDBKeysFromValuesAndKeyPath(
      int id, const std::vector<SerializedScriptValue>& serialized_values,
      const string16& key_path);

  
  
  bool StartInjectIDBKey(const IndexedDBKey& key,
                         const SerializedScriptValue& value,
                         const string16& key_path);

  
  
  bool StartJSONParsing(const std::string& json);

  
  
  bool StartBatchMode();

  
  void EndBatchMode();

 protected:
  
  virtual FilePath GetUtilityProcessCmd();

 private:
  
  
  bool StartProcess(const FilePath& exposed_dir);

  
  virtual bool OnMessageReceived(const IPC::Message& message);

  
  virtual void OnProcessCrashed(int exit_code);
  virtual bool CanShutdown();

  
  scoped_refptr<Client> client_;
  BrowserThread::ID client_thread_id_;
  
  
  bool is_batch_mode_;

  DISALLOW_COPY_AND_ASSIGN(UtilityProcessHost);
};

#endif  
