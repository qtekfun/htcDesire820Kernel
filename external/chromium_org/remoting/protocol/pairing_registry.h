// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_PAIRING_REGISTRY_H_
#define REMOTING_PROTOCOL_PAIRING_REGISTRY_H_

#include <map>
#include <queue>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"

namespace base {
class DictionaryValue;
class ListValue;
class SingleThreadTaskRunner;
}  

namespace tracked_objects {
class Location;
}  

namespace remoting {
namespace protocol {

class PairingRegistry : public base::RefCountedThreadSafe<PairingRegistry> {
 public:
  struct Pairing {
    Pairing();
    Pairing(const base::Time& created_time,
            const std::string& client_name,
            const std::string& client_id,
            const std::string& shared_secret);
    ~Pairing();

    static Pairing Create(const std::string& client_name);
    static Pairing CreateFromValue(const base::DictionaryValue& pairing);

    scoped_ptr<base::DictionaryValue> ToValue() const;

    bool operator==(const Pairing& other) const;

    bool is_valid() const;

    base::Time created_time() const { return created_time_; }
    std::string client_id() const { return client_id_; }
    std::string client_name() const { return client_name_; }
    std::string shared_secret() const { return shared_secret_; }

   private:
    base::Time created_time_;
    std::string client_name_;
    std::string client_id_;
    std::string shared_secret_;
  };

  
  typedef std::map<std::string, Pairing> PairedClients;

  
  typedef base::Callback<void(bool success)> DoneCallback;
  typedef base::Callback<void(scoped_ptr<base::ListValue> pairings)>
      GetAllPairingsCallback;
  typedef base::Callback<void(Pairing pairing)> GetPairingCallback;

  static const char kCreatedTimeKey[];
  static const char kClientIdKey[];
  static const char kClientNameKey[];
  static const char kSharedSecretKey[];

  
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual scoped_ptr<base::ListValue> LoadAll() = 0;

    
    virtual bool DeleteAll() = 0;

    
    virtual Pairing Load(const std::string& client_id) = 0;

    
    virtual bool Save(const Pairing& pairing) = 0;

    
    virtual bool Delete(const std::string& client_id) = 0;
  };

  PairingRegistry(
      scoped_refptr<base::SingleThreadTaskRunner> delegate_task_runner,
      scoped_ptr<Delegate> delegate);

  
  
  
  
  
  Pairing CreatePairing(const std::string& client_name);

  
  
  
  void GetPairing(const std::string& client_id,
                  const GetPairingCallback& callback);

  
  void GetAllPairings(const GetAllPairingsCallback& callback);

  
  
  
  void DeletePairing(const std::string& client_id,
                     const DoneCallback& callback);

  
  void ClearAllPairings(const DoneCallback& callback);

 protected:
  friend class base::RefCountedThreadSafe<PairingRegistry>;
  virtual ~PairingRegistry();

  
  virtual void PostTask(
      const scoped_refptr<base::SingleThreadTaskRunner>& task_runner,
      const tracked_objects::Location& from_here,
      const base::Closure& task);

 private:
  FRIEND_TEST_ALL_PREFIXES(PairingRegistryTest, AddPairing);
  friend class NegotiatingAuthenticatorTest;

  
  void AddPairing(const Pairing& pairing);

  
  void DoLoadAll(
      const protocol::PairingRegistry::GetAllPairingsCallback& callback);
  void DoDeleteAll(
      const protocol::PairingRegistry::DoneCallback& callback);
  void DoLoad(
      const std::string& client_id,
      const protocol::PairingRegistry::GetPairingCallback& callback);
  void DoSave(
      const protocol::PairingRegistry::Pairing& pairing,
      const protocol::PairingRegistry::DoneCallback& callback);
  void DoDelete(
      const std::string& client_id,
      const protocol::PairingRegistry::DoneCallback& callback);

  
  
  void InvokeDoneCallbackAndScheduleNext(
      const DoneCallback& callback, bool success);
  void InvokeGetPairingCallbackAndScheduleNext(
      const GetPairingCallback& callback, Pairing pairing);
  void InvokeGetAllPairingsCallbackAndScheduleNext(
      const GetAllPairingsCallback& callback,
      scoped_ptr<base::ListValue> pairings);

  
  void SanitizePairings(const GetAllPairingsCallback& callback,
                        scoped_ptr<base::ListValue> pairings);

  
  void ServiceOrQueueRequest(const base::Closure& request);
  void ServiceNextRequest();

  
  scoped_refptr<base::SingleThreadTaskRunner> caller_task_runner_;

  
  
  
  scoped_refptr<base::SingleThreadTaskRunner> delegate_task_runner_;

  scoped_ptr<Delegate> delegate_;

  std::queue<base::Closure> pending_requests_;

  DISALLOW_COPY_AND_ASSIGN(PairingRegistry);
};

}  
}  

#endif  
