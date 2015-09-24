// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_GCM_GCM_CLIENT_H_
#define GOOGLE_APIS_GCM_GCM_CLIENT_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "google_apis/gcm/base/gcm_export.h"

namespace base {
class TaskRunner;
}

namespace gcm {

class GCM_EXPORT GCMClient {
 public:
  enum Result {
    
    SUCCESS,
    
    INVALID_PARAMETER,
    
    
    ASYNC_OPERATION_PENDING,
    
    NETWORK_ERROR,
    
    SERVER_ERROR,
    
    TTL_EXCEEDED,
    
    UNKNOWN_ERROR
  };

  
  typedef std::map<std::string, std::string> MessageData;

  
  struct GCM_EXPORT OutgoingMessage {
    OutgoingMessage();
    ~OutgoingMessage();

    
    std::string id;
    
    int time_to_live;
    MessageData data;
  };

  
  struct GCM_EXPORT IncomingMessage {
    IncomingMessage();
    ~IncomingMessage();

    MessageData data;
  };

  
  struct GCM_EXPORT CheckInInfo {
    CheckInInfo() : android_id(0), secret(0) {}
    bool IsValid() const { return android_id != 0 && secret != 0; }
    void Reset() {
      android_id = 0;
      secret = 0;
    }

    uint64 android_id;
    uint64 secret;
  };

  
  
  class Delegate {
   public:
    
    
    
    virtual void OnCheckInFinished(const CheckInInfo& checkin_info,
                                   Result result) = 0;

    
    
    
    
    virtual void OnRegisterFinished(const std::string& app_id,
                                    const std::string& registration_id,
                                    Result result) = 0;

    
    
    
    
    
    virtual void OnSendFinished(const std::string& app_id,
                                const std::string& message_id,
                                Result result) = 0;

    
    
    
    virtual void OnMessageReceived(const std::string& app_id,
                                   const IncomingMessage& message) = 0;

    
    
    virtual void OnMessagesDeleted(const std::string& app_id) = 0;

    
    
    
    
    virtual void OnMessageSendError(const std::string& app_id,
                                    const std::string& message_id,
                                    Result result) = 0;

    
    
    
    virtual CheckInInfo GetCheckInInfo() const = 0;

    
    
    virtual void OnLoadingCompleted() = 0;

    
    
    virtual base::TaskRunner* GetFileTaskRunner() = 0;
  };

  
  
  static GCMClient* Get();

  
  static void SetForTesting(GCMClient* client);

  
  
  
  
  
  virtual void CheckIn(const std::string& username, Delegate* delegate) = 0;

  
  
  
  
  
  
  
  
  virtual void Register(const std::string& username,
                        const std::string& app_id,
                        const std::string& cert,
                        const std::vector<std::string>& sender_ids) = 0;

  
  
  
  
  
  virtual void Unregister(const std::string& username,
                          const std::string& app_id) = 0;

  
  
  
  
  
  
  virtual void Send(const std::string& username,
                    const std::string& app_id,
                    const std::string& receiver_id,
                    const OutgoingMessage& message) = 0;

  
  virtual bool IsLoading() const = 0;

 protected:
  virtual ~GCMClient() {}
};

}  

#endif  
