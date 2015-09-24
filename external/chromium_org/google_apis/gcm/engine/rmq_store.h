// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_GCM_ENGINE_RMQ_STORE_H_
#define GOOGLE_APIS_GCM_ENGINE_RMQ_STORE_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "google_apis/gcm/base/gcm_export.h"

namespace base {
class FilePath;
class SequencedTaskRunner;
}  

namespace google {
namespace protobuf {
class MessageLite;
}  
}  

namespace gcm {

class MCSMessage;

class GCM_EXPORT RMQStore {
 public:
  
  struct GCM_EXPORT LoadResult {
    LoadResult();
    ~LoadResult();

    bool success;
    uint64 device_android_id;
    uint64 device_security_token;
    std::vector<std::string> incoming_messages;
    std::map<std::string, google::protobuf::MessageLite*>
        outgoing_messages;
  };

  typedef std::vector<std::string> PersistentIdList;
  
  typedef base::Callback<void(const LoadResult& result)> LoadCallback;
  typedef base::Callback<void(bool success)> UpdateCallback;

  RMQStore(const base::FilePath& path,
           scoped_refptr<base::SequencedTaskRunner> blocking_task_runner);
  ~RMQStore();

  
  void Load(const LoadCallback& callback);

  
  
  
  
  void Destroy(const UpdateCallback& callback);

  
  void SetDeviceCredentials(uint64 device_android_id,
                            uint64 device_security_token,
                            const UpdateCallback& callback);

  
  void AddIncomingMessage(const std::string& persistent_id,
                          const UpdateCallback& callback);
  void RemoveIncomingMessage(const std::string& persistent_id,
                             const UpdateCallback& callback);
  void RemoveIncomingMessages(const PersistentIdList& persistent_ids,
                              const UpdateCallback& callback);

  
  
  void AddOutgoingMessage(const std::string& persistent_id,
                          const MCSMessage& message,
                          const UpdateCallback& callback);
  void RemoveOutgoingMessage(const std::string& persistent_id,
                             const UpdateCallback& callback);
  void RemoveOutgoingMessages(const PersistentIdList& persistent_ids,
                              const UpdateCallback& callback);

 private:
  class Backend;

  scoped_refptr<Backend> backend_;
  scoped_refptr<base::SequencedTaskRunner> blocking_task_runner_;

  DISALLOW_COPY_AND_ASSIGN(RMQStore);
};

}  

#endif  
